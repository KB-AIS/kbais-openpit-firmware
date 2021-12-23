// qt
#include <QApplication>
#include <QMetaType>
// oss
#include <boost/di.hpp>
#include <readerwriterqueue/readerwriterqueue.h>
#include <spdlog/spdlog.h>

#include "caching/messages_caching_service.h"
#include "gps_device_controller.h"
#include "host_wrapper.h"
#include "messaging/aux_immediate_messages_map_service.h"
#include "messaging/aux_recurrent_messages_map_service.h"
#include "messaging/collectors/immediate_messages_collector.h"
#include "messaging/collectors/messages_collectors_adapter.h"
#include "messaging/collectors/recurrent_messages_collector.h"
#include "messaging/messages_batch.h"
#include "networking/senders/tcp_message_senders_manager.h"
#include "networking/communicators/swom_protocol_communicator.h"
#include "networking/senders/message_sender.h"
#include "persisting/configuration/database_configuration.h"
#include "utils/boost_di_extensions.h"

namespace di = boost::di;

using namespace KbAis::Cfw::Sensors::Gps;

// TODO: Remove
struct MessageSendersManagerBootstraper {

    MessageSendersManagerBootstraper(BaseMessageSendersManager& manager) {
        QList<MessageSenderConfiguration> configurations {
            {
                "10.214.1.208",
                9900,
                "SWOM",
                std::chrono::milliseconds { 10000 },
            },
        };

        manager.handleConfigurationChanged(configurations);
    }

};

int main(int argc, char* argv[]) {
    spdlog::set_pattern("[%T %z][%^%L%$][trd %t][%@][%u] %v");
    spdlog::set_level(spdlog::level::trace);

    spdlog::info("Setup AUX application");
    QApplication app(argc, argv);

    Caching::Configuration::configureConnection();

    qMetaTypeId<MessageSenderStatusChanged>();

    const auto services = di::make_injector(
        di::bind<AuxImmediateMessagesMapService>()
            .in(di::singleton),
        di::bind<AuxRecurrentMessagesMapService>()
            .in(di::singleton),
        di::bind<BaseGpsDeviceController>()
            .to<SerialPortGpsDeviceController>()
            .in(di::singleton),
        di::bind<BaseMessageSendersManager>()
            .to<TcpMessageSendersManager>()
            .in(di::singleton),
        di::bind<HostWrapper>()
            .in(di::singleton),
        di::bind<IMessagesCachingService>()
            .to<MessagesCachingService>()
            .in(di::singleton),
        di::bind<ImmediateMessagesCollector>()
            .in(di::singleton),
        di::bind<MessagesCollectorsAdapter>()
            .in(di::singleton),
        di::bind<RecurrentMessagesCollector>()
            .in(di::singleton),
        di::bind<moodycamel::BlockingReaderWriterQueue<MessagesBatch>>()
            .in(di::singleton)
    );

    eagerSingletons(services);
    services.create<MessageSendersManagerBootstraper>();

    spdlog::info("Startup AUX application");
    return app.exec();
}
