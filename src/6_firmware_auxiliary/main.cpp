#include <forms/main_form.h>

// Qt
#include <QApplication>
// Sensors GPS
#include <gps_sensor_thread.h>
// OSS
#include <plog/Init.h>
#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>

using namespace Sensors::Gps;

static void setup_logging() {
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug).addAppender(&consoleAppender);
}

int main(int argc, char *argv[]) {
    setup_logging();

    PLOGI << "Starting AUX application";

    QApplication app(argc, argv);

    // Sensors
    auto gps_sensor_thread = new class gps_sensor_thread(&app);
    //gps_sensor_thread->moveToThread(gps_sensor_thread);

    // UI
    main_form main_form;

    // Connect
    QObject::connect(
        gps_sensor_thread,
        &gps_sensor_thread::update_gps_data_signal,
        &main_form,
        &main_form::update_gps_data_slot
    );

    gps_sensor_thread->start();

    main_form.show();

    return app.exec();
}
