#include "aux_recurrent_messages_mapper.h"
#include "aux_immediate_messages_mapper.h"
#include "commands/setup_time_handler.h"
#include "device_message.h"
#include "messages_collectors_adapter.h"
#include "host_wrapper.h"
#include "immediate_messages_collector.h"
// Qt
#include <QApplication>
#include <QHash>
#include <QMetaType>
// plog
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>

using namespace Sensors::Gps;

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

    GpsDeviceController gps_controller { };

    SetupTimeHandler setup_time_handler { };

    HostWrapper host { gps_controller };

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

    AuxImmediateMessagesMapper auxImmediateEventMapper { immediate_event_collector, &host };

    MessagesCollectorsAdapter event_collectors_adapter {
        recurrent_event_collector,
        immediate_event_collector
    };

    PLOGD << "[MAIN] Do work in thead: " << QThread::currentThread();

    PLOGI << "Startup AUX application";
    return app.exec();
}
