#include <presentation/main_presenter.h>

// sensors\gps
#include <gps_sensor_thread.h>

// qt
#include <QApplication>
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

    // Sensors
    gps_sensor_thread gps_sensor_thread;
    gps_sensor_thread.moveToThread(&gps_sensor_thread);

    // UI
    main_presenter main_presenter;

    // Connect
    QObject::connect(
        &gps_sensor_thread, &gps_sensor_thread::update_gps_data_signal,

        &main_presenter, &main_presenter::update_gps_data_slot
    );

    gps_sensor_thread.start();

    main_presenter.show();

    PLOGI << "Starup AUX application";
    return app.exec();
}
