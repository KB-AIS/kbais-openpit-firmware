// std
#include <memory>
// qt
#include <QApplication>
#include <QObject>
#include <QThread>
// oss
#include <boost/di.hpp>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>

#include "core/notification_handlers/setup_time_handler.h"
#include "database_configuration.h"
#include "gps_device_controller.h"
#include "host_wrapper.h"
#include "networking/base_message_senders_manager.h"
#include "networking/message_senders_manager.h"
#include "networking/swom_protocol_communicator.h"

namespace di = boost::di;

using namespace Caching::Configuration;
using namespace KbAis::Cfw::Core;
using namespace KbAis::Cfw::Networking;
using namespace KbAis::Cfw::Sensors::Gps;

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
                MessageSenderConfiguration {
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

struct NotificationHandlerConfiguration {
    NotificationHandlerConfiguration(
        BaseGpsDeviceController& gspController,
        SetupTimerHandler& setupTimeHandler
    ) {
        QObject::connect(
          &gspController, &BaseGpsDeviceController::notifyGpsDataUpdated,

          &setupTimeHandler, &SetupTimerHandler::handleGpsDataUdpated);
    }
};

struct AppConfiguration {
    AppConfiguration() {
        configureConnection();
    }
};

void setupLogging() {
    using namespace plog;

    static ConsoleAppender<TxtFormatter> console_appender;

    init(debug).addAppender(&console_appender);
}

int main(int argc, char *argv[]) {
    setupLogging();

    PLOGI << "Setup AUX application";
    QApplication app(argc, argv);

    const auto injector = boost::di::make_injector(
        // Sensors
        di::bind<SerialPortGpsDeviceController>().to<SerialPortGpsDeviceController>().in(di::singleton),
        // Caching

        // Networking
        di::bind<BaseMessageSendersManager>().to<TcpMessageSendersManager>().in(di::singleton),
        // System handlers
        di::bind<SetupTimerHandler>().to<SetupTimeHandlerImpl>().in(di::singleton)
    );

    injector.create<ThreadWrapper>();
    injector.create<NotificationHandlerConfiguration>();
    injector.create<HostWrapper>();
    injector.create<AppConfiguration>();

    PLOGI << "Startup AUX application";
    return app.exec();
}
