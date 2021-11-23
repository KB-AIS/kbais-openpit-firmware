#include <commands/setup_time_handler.h>
#include <host_wrapper.h>


// qt
#include <QApplication>
#include <QObject>
// plog
#include <plog/Init.h>
#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>

using namespace Sensors::Gps;

static void setup_logging() {
    using namespace plog;

    static ConsoleAppender<TxtFormatter> console_appender;

    init(debug).addAppender(&console_appender);
}

int main(int argc, char *argv[]) {
    setup_logging();

    PLOGI << "Setup AUX application";
    QApplication app(argc, argv);

    GpsDeviceController gpsController;

    HostWrapper host { gpsController };

    SetupTimeHandler setupTimeHandler;

    QObject::connect(
        &gpsController, &GpsDeviceController::update_gps_data_signal,

        &setupTimeHandler, &SetupTimeHandler::handle_slot
    );

    PLOGI << "Starup AUX application";
    return app.exec();
}
