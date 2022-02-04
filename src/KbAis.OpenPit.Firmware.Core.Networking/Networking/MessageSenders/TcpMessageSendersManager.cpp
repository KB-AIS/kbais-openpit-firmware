#include "TcpMessageSendersManager.h"

// qt
#include "QMetaEnum"
// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

// Utils.TrdParty.JsonQt
#include "JsonQt.h"
// Utils.TrdParty.RxQt
#include "RxQt.h"
#include <variant>

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds RESTART_MESSAGE_SENDERS_PERIOD { 5s };

template<typename QEnum>
QString qt_enum_to_string(const QEnum value) {
    return QMetaEnum::fromType<QEnum>().valueToKey(value);
}

TcpMessageSendersManager::TcpMessageSendersManager(
    IRxConfigurationChangePublisher& configurationPublisher
)
    :   QObject()
    ,   m_configuration_publisher(configurationPublisher)
    ,   m_subject({ })
{

}

void
TcpMessageSendersManager::start_work_on(rxcpp::observe_on_one_worker& scheduler) {
    m_subscriptions = rxcpp::composite_subscription();

    auto const on_configuration_changed_f =
        std::bind(&TcpMessageSendersManager::on_configuration_changed, this, std::placeholders::_1);
    m_configuration_publisher.getChangeObservable("networking")
        // Workaround: nasty way to prevent fireing on file change twice
        .sample_with_time(500ms, scheduler)
        .subscribe(
            m_subscriptions
        ,   on_configuration_changed_f
        ,   [](std::exception_ptr e) {
                PLOGE << "An exception occured " << rxcpp::util::what(e);
            }
        );

    auto const on_message_senders_restart_required_f =
             std::bind(&TcpMessageSendersManager::on_message_senders_restart_required, this);
    rxcpp::observable<>::interval(RESTART_MESSAGE_SENDERS_PERIOD, scheduler)
        .subscribe(m_subscriptions, on_message_senders_restart_required_f);
}

rxcpp::observable<std::vector<MessageSenderDiagInfo>>
TcpMessageSendersManager::get_diag_observable() const {
    return m_subject.get_observable();
}

void
TcpMessageSendersManager::on_configuration_changed(
    AppConfiguration configuration
) {
    m_status_subscriptions.unsubscribe();
    m_status_subscriptions = rxcpp::composite_subscription();

    m_message_sender_states.clear();

    m_message_sender_configurations.clear();

    // Stop message senders
    m_message_senders.clear();

    m_message_sender_configurations = configuration.j_object.at("/servers"_json_pointer)
        | ranges::views::transform([](nlohmann::json x) {
            TcpMessageSenderConfiguration msc {
                x.at("server_name").get<QString>()
            ,   x.at("server_port").get<quint16>()
            ,   MessageSenderProtocol::Swom
            ,   x.at("enabled").get<bool>()
            ,   std::chrono::seconds(x.at("send_interval").get<quint16>())
            };

            return std::make_pair(msc.GetMessageSenderName(), std::move(msc));
        })
        | ranges::to<MessageSenderConfigurations_t>();

    ranges::for_each(
        m_message_sender_configurations
    |   ranges::views::values
    ,   [&](TcpMessageSenderConfiguration x) {
            const auto id = x.GetMessageSenderName();

            m_message_senders[id].reset(new TcpMessageSender { id });
            // Set default state: unconnected
            m_message_sender_states[id] = { };

            const auto on_message_sender_state_changed_f =
                std::bind(&TcpMessageSendersManager::on_message_sender_state_changed, this, std::placeholders::_1);
            rxqt::from_signal(m_message_senders[id].get(), &TcpMessageSender::StateChanged)
                .subscribe(m_status_subscriptions, on_message_sender_state_changed_f);
        }
    );
}

void
TcpMessageSendersManager::on_message_senders_restart_required() {
    ranges::for_each(
        m_message_sender_states
    |   ranges::views::filter([&](const MessageSenderState_t& x) {
            const auto has_message_sender_invalid_state =
                x.second.socket_state == QAbstractSocket::UnconnectedState;

            return has_message_sender_invalid_state;
        })
    |   ranges::views::keys
    ,   [&](const MessageSenderId_t& x) {
            m_message_senders[x]->Restart(m_message_sender_configurations[x]);
        }
    );
}

void
TcpMessageSendersManager::on_message_sender_state_changed(
    TcpMessageSenderStateChanged notification
) {
    m_message_sender_states[notification.message_sender_name] = notification.state;

    m_subject.get_subscriber().on_next(
        m_message_sender_states
    |   ranges::views::transform([&](const MessageSenderState_t& x) {
            return MessageSenderDiagInfo {
                x.first
            ,   qt_enum_to_string(x.second.socket_state)
            ,   x.second.socket_error != QAbstractSocket::UnknownSocketError
            ?   std::make_optional(qt_enum_to_string(x.second.socket_error))
            :   std::nullopt
            };
        })
    |   ranges::to<std::vector<MessageSenderDiagInfo>>()
    );
}
