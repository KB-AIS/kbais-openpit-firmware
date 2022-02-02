#include "TcpMessageSendersManager.h"

// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

// Utils.TrdParty.JsonQt
#include "JsonQt.h"

using namespace std::chrono_literals;

TcpMessageSendersManager::TcpMessageSendersManager(
    IRxConfigurationChangePublisher& configurationPublisher
)
    :   QObject()
    ,   m_configuration_publisher(configurationPublisher)
{

}

void
TcpMessageSendersManager::start_work_on(rxcpp::observe_on_one_worker& scheduler) {
    m_subscriptions = rxcpp::composite_subscription();

    m_configuration_publisher.getChangeObservable("networking")
        // Workaround: nasty way to prevent fireing on file change twice
        .sample_with_time(500ms, scheduler)
        .subscribe(
            m_subscriptions
        ,   [this](AppConfiguration x) {
                on_configuration_changed(x);
            }
        ,   [](std::exception_ptr e) {
                PLOGE << "An exception occured " << rxcpp::util::what(e);
            }
        ,   [] {
                PLOGD << "DONE";
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

    // TODO: Gracefully stop all senders
    m_message_senders.clear();

    auto message_sender_configurations = j_object
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
        message_sender_configurations | ranges::views::values
        , [&](MessageSenderConfiguration x) {
            const auto message_sender_name = x.get_name();
            m_message_senders[message_sender_name].reset(new TcpMessageSender { });
            m_message_senders[message_sender_name]->restart(x);
        }
    );
}

void
TcpMessageSendersManager::restart_message_senders() {

}

