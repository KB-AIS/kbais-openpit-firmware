// std
#include <memory>
// qt
#include <QApplication>
#include <QDateTime>
#include <QHash>
#include <QHostAddress>
#include <QMetaType>
#include <QtConcurrent/QtConcurrent>
// oss
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>
#include <readerwriterqueue.h>

#include "aux_immediate_messages_mapper.h"
#include "aux_recurrent_messages_mapper.h"
#include "commands/setup_time_handler.h"
#include "database_factory.h"
#include "device_message.h"
#include "host_wrapper.h"
#include "immediate_messages_collector.h"
#include "messages_caching_service.h"
#include "messages_collectors_adapter.h"
#include "networking/base_protocol_communicator.h"
#include "networking/swom_protocol_communicator.h"
#include "networking/message_sender.h"
#include "networking/message_senders_manager.h"

using namespace KbAis::Cfw::Networking;

using namespace moodycamel;

void setupLogging() {
    using namespace plog;

    static ConsoleAppender<TxtFormatter> console_appender;

    init(debug).addAppender(&console_appender);
}

void setupMetaTypes() {
    qMetaTypeId<DeviceMessage>();
}

int main(int argc, char *argv[]) {
    setupLogging();
    setupMetaTypes();

    PLOGI << "Setup AUX application";
    QApplication app(argc, argv);

    Caching::Configuration::configureConnection();

    Sensors::Gps::GpsDeviceController gspController;

    SetupTimeHandler setupTimeHandler;

    HostWrapper host { gspController };

    QObject::connect(
       &gspController, &Sensors::Gps::GpsDeviceController::update_gps_data_signal,

       &setupTimeHandler, &SetupTimeHandler::handle_slot
    );

    RecurrentMessagesCollector recurrentMessagesCollector;

    AuxRecurrentEventMapper auxRecurrentEventMapper {
        recurrentMessagesCollector,
        gspController
    };

    ImmediateMessagesCollector immediateMessagesCollector { };

    AuxImmediateMessagesMapper auxImmediateEventMapper {
        immediateMessagesCollector, &host
    };

    BlockingReaderWriterQueue<DeviceMessage> msgsQueue;

    MessagesCollectorsAdapter msgsCollectorsAdapter {
        recurrentMessagesCollector,
        immediateMessagesCollector,
        msgsQueue
    };

    MessagesCachingService msgsCachingService {
        msgsQueue
    };

    QList<MessageSenderConfiguration> configurations {
        {
            "10.214.1.247",
            9900,
            std::chrono::milliseconds { 10000 },
            std::make_shared<SwomProtocolCommunicator>()
        },
        {
            "10.214.1.208",
            9900,
            std::chrono::milliseconds { 5000 },
            std::make_shared<SwomProtocolCommunicator>()
        }
    };

    MessageSendersManager messageSendersManager;
    messageSendersManager.handleConfigurationChanged(configurations);

    PLOGI << "Startup AUX application";
    return app.exec();
}
