#include "aux_immediate_messages_mapper.h"
#include "aux_recurrent_messages_mapper.h"
#include "commands/setup_time_handler.h"
#include "database_factory.h"
#include "device_message.h"
#include "host_wrapper.h"
#include "immediate_messages_collector.h"
#include "messages_caching_service.h"
#include "messages_collectors_adapter.h"
#include "messages_sending_service.h"
// Qt
#include <QApplication>
#include <QtConcurrent/QtConcurrent>
#include <QHash>
#include <QMetaType>
#include <QDateTime>
// plog
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>
// OSS
#include <rigtorp/MPMCQueue.h>
#include <json.hpp>
#include <readerwriterqueue.h>

using MessagesQueue = rigtorp::mpmc::Queue<DeviceMessage>;

using namespace Caching::Configuration;

using namespace Sensors::Gps;

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

    //configureConnection();

    GpsDeviceController gps_controller { };

    SetupTimeHandler setup_time_handler { };

//    HostWrapper host { gps_controller };

    QObject::connect(
       &gps_controller, &GpsDeviceController::update_gps_data_signal,

       &setup_time_handler, &SetupTimeHandler::handle_slot
    );

    RecurrentMessagesCollector recurrent_event_collector { };

    AuxRecurrentEventMapper aux_reccurrent_event_mapper {
        recurrent_event_collector,
        gps_controller
    };

    ImmediateMessagesCollector immediate_event_collector { };

//    AuxImmediateMessagesMapper auxImmediateEventMapper {
//        immediate_event_collector,
//        &host
//    };

    MessagesQueue msgsQueue { 5 };
    BlockingReaderWriterQueue<DeviceMessage> q;

    MessagesCollectorsAdapter event_collectors_adapter {
        recurrent_event_collector,
        immediate_event_collector,
        q
    };

    auto f = QtConcurrent::run([&] {
        MessagesSendingService msgsSendingSrv {};

        while (1) {
            DeviceMessage v;
            q.wait_dequeue(v);
            msgsSendingSrv.sendMessages({ v });
        }
    });

    //MessagesCachingService msgsCachingSrv { msgsQueue, msgsSendingSrv };

    PLOGI << "Startup AUX application";
    return app.exec();
}
