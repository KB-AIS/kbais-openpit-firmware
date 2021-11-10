#include <host_wrapper.h>

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

    host_wrapper host;

    PLOGI << "Starup AUX application";
    return app.exec();
}
