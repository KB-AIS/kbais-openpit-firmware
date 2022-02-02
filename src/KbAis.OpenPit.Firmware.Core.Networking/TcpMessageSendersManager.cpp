#include "TcpMessageSendersManager.h"

// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

// Utils.TrdParty.JsonQt
#include "JsonQt.h"

TcpMessageSendersManager::TcpMessageSendersManager(
    IRxConfigurationChangePublisher& configurationPublisher
)
    :   QObject()
    ,   mConfigurationPublisher(configurationPublisher)
{
    startWorkOn();
}

void
TcpMessageSendersManager::startWorkOn(/*rxcpp::observe_on_one_worker& scheduler*/) {
    mSubs = rxcpp::composite_subscription();

    mConfigurationPublisher.getChangeObservable("networking")
        .observe_on(rxcpp::identity_current_thread())
        .subscribe(
            mSubs
        ,   [this](AppConfiguration x) {
                on_configuration_changed(x);
            }
        ,   [](std::exception_ptr e) {
                PLOGE << "An exception occured";
            }
        );
}

rxcpp::observable<QVector<MessageSenderStatus>>
TcpMessageSendersManager::get_status_observable() const {

}

void
TcpMessageSendersManager::on_configuration_changed(
    AppConfiguration configuration
) {
    const auto j = configuration.j_value.at("/servers"_json_pointer);

    ranges::for_each(j, [](nlohmann::basic_json<> x) {
        const auto host = x.at("server_name").get<QString>();
        const auto port = x.at("server_port").get<quint16>();
        // Format message sender name as host:port
        const auto message_sender_name = QString("%1:%2").arg(host).arg(port);

        MessageSenderConfiguration message_sender_configuration {
            message_sender_name
        ,   host
        ,   port
        ,   "swom"
        ,   x.at("enabled").get<bool>()
        ,   std::chrono::seconds(x.at("send_interval").get<quint32>())
        };

        TcpMessageSender message_sender;
    });
}

void
TcpMessageSendersManager::restartMessageSendersWithError() {

}
