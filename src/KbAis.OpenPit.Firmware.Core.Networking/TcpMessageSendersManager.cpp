#include "TcpMessageSendersManager.h"

// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

// Utils.TrdParty.JsonQt
#include "JsonQt.h"
// Utils.TrdParty.RxQt
#include "RxQt.h"

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds RESTART_MESSAGE_SENDERS_PERIOD { 5s };

TcpMessageSendersManager::TcpMessageSendersManager(
    IRxConfigurationChangePublisher& configurationPublisher
)
    :   QObject()
    ,   m_configuration_publisher(configurationPublisher)
{

}

void
TcpMessageSendersManager::start_work_on(rxcpp::observe_on_one_worker& scheduler) {
    m_main_subs = rxcpp::composite_subscription();

    m_configuration_publisher.getChangeObservable("networking")
        // Workaround: nasty way to prevent fireing on file change twice
        .sample_with_time(500ms, scheduler)
        .subscribe(
            m_main_subs
        ,   [this](AppConfiguration x) {
                on_configuration_changed(x);
            }
        ,   [](std::exception_ptr e) {
                PLOGE << "An exception occured " << rxcpp::util::what(e);
            }
        );

    rxcpp::observable<>::interval(RESTART_MESSAGE_SENDERS_PERIOD, scheduler)
        .subscribe(
            m_main_subs
        ,   [this](auto) {
                on_message_senders_restart_required();
            }
        );
}

rxcpp::observable<QVector<MessageSenderStatus>>
TcpMessageSendersManager::get_diag_observable() const {

}

void
TcpMessageSendersManager::on_configuration_changed(
    AppConfiguration configuration
) {
    const auto j_object = configuration.j_object.at("/servers"_json_pointer);

    if (m_message_sender_status_changed_subs.is_subscribed()) {
        m_message_sender_status_changed_subs.unsubscribe();
    }

    m_message_sender_status_changed_subs = rxcpp::composite_subscription();

    // TODO: Gracefully stop all senders
    m_message_senders.clear();   

    m_message_sender_configurations = j_object
        | ranges::views::transform([](nlohmann::json x) {
            MessageSenderConfiguration message_sender_configuration {
                x.at("server_name").get<QString>()
            ,   x.at("server_port").get<quint16>()
            ,   "swom"
            ,   x.at("enabled").get<bool>()
            ,   std::chrono::seconds(x.at("send_interval").get<quint16>())
            };

            return std::make_pair(
                message_sender_configuration.get_name()
            ,   std::move(message_sender_configuration)
            );
        })
        | ranges::to<MessageSenderConfigurations_t>();

    ranges::for_each(
        m_message_sender_configurations | ranges::views::values
    ,   [&](MessageSenderConfiguration x) {
            const auto id = x.get_name();

            m_message_senders[id].reset(new TcpMessageSender { id });
            // Set default state: unconnected
            m_message_sender_states[id] = { };

            rxqt::from_signal(
                m_message_senders[id].get()
            ,   &TcpMessageSender::state_changed
            )
                .observe_on(rxcpp::identity_current_thread())
                .subscribe(
                    m_message_sender_status_changed_subs
                ,   [&](TcpMessageSenderStateChanged x) {
                        m_message_sender_states[x.message_sender_name].socket_state = x.state.socket_state;
                    }
                );
        }
    );
}

void
TcpMessageSendersManager::on_message_senders_restart_required() {
    ranges::for_each(
        m_message_sender_states
    ,   [&](std::pair<MessageSenderId_t, TcpMessageSenderState> x) {
            const auto state = &x.second;

            const auto has_message_sender_valid_state =
                state->socket_state != QAbstractSocket::UnconnectedState;

            if (has_message_sender_valid_state) return;

            m_message_senders[x.first]->restart(m_message_sender_configurations[x.first]);
        }
    );
}
