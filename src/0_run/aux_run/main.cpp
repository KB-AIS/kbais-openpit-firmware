// qt
#include <QApplication>
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
#include "networking/senders/message_senders_manager.h"
#include "networking/communicators/swom_protocol_communicator.h"
#include "persisting/configuration/database_configuration.h"
#include "utils/boost_di_extensions.h"

namespace di = boost::di;

using namespace KbAis::Cfw::Sensors::Gps;

// TODO: Remove
struct ThreadWrapper {

public:
    ThreadWrapper(BaseMessageSendersManager& messageSendersManager) {
        hostThread = new QThread();
        hostThread->setObjectName("Wrapper host thread");

        messageSendersManager.moveToThread(hostThread);

        QObject::connect(
            hostThread, &QThread::started,
            &messageSendersManager, [&] {
            QList<MessageSenderConfiguration> configurations {
                {
                    "10.214.1.208",
                    9900,
                    std::chrono::milliseconds { 10000 },
                    QSharedPointer<SwomProtocolCommunicator>::create()
                },
            };

            messageSendersManager.handleConfigurationChanged(configurations);
        });

        QObject::connect(
            hostThread, &QThread::finished,
            &messageSendersManager, &QThread::deleteLater);

        hostThread->start();
    }

private:
    QThread* hostThread;

};


int main(int argc, char* argv[]) {
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^%L%$] [thread %t] %v");
    spdlog::set_level(spdlog::level::trace);

    spdlog::info("Setup AUX application");
    QApplication app(argc, argv);

    Caching::Configuration::configureConnection();

    const auto services = di::make_injector(
        di::bind<BaseGpsDeviceController>()
            .to<SerialPortGpsDeviceController>().in(di::singleton),

        di::bind<HostWrapper>(),

        di::bind<moodycamel::BlockingReaderWriterQueue<MessagesBatch>>().in(di::singleton),

        di::bind<ImmediateMessagesCollector>().in(di::singleton),
        di::bind<RecurrentMessagesCollector>().in(di::singleton),
        di::bind<MessagesCollectorsAdapter>().in(di::singleton),
        di::bind<AuxImmediateMessagesMapService>().in(di::singleton),
        di::bind<AuxRecurrentMessagesMapService>().in(di::singleton),

        di::bind<IMessagesCachingService>()
            .to<MessagesCachingService>().in(di::singleton),

        di::bind<BaseMessageSendersManager>().to<TcpMessageSendersManager>().in(di::singleton)
    );

    services.create<ThreadWrapper>();
    eagerSingletons(services);

    spdlog::info("Startup AUX application");
    return app.exec();
}
