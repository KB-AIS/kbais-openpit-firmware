#include "commands/setup_time_handler.h"
#include "host_wrapper.h"

// qt
#include <QApplication>
#include <QObject>
// plog
#include <plog/Init.h>
#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>

using namespace Sensors::Gps;

void setup_logging() {
    using namespace plog;

    static ConsoleAppender<TxtFormatter> console_appender;

    init(debug).addAppender(&console_appender);
}

int main(int argc, char *argv[]) {
    setup_logging();

    PLOGI << "Setup AUX application";
    QApplication app(argc, argv);

    // TODO: Move composition initialization into abstruction (DI container, mb?).
    GpsDeviceController gpsController;

    SetupTimeHandler setupTimeHandler { };

    HostWrapper host { gpsController };

    QObject::connect(
        &gpsController, &GpsDeviceController::update_gps_data_signal,

        &setupTimeHandler, &SetupTimeHandler::handle_slot
    );

    PLOGI << "Startup AUX application";
    return app.exec();
}
