// qt
#include <QApplication>
// oss
#include <boost/di.hpp>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>

namespace di = boost::di;

void setupLogging() {
    using namespace plog;

    static ConsoleAppender<TxtFormatter> console_appender;

    init(verbose).addAppender(&console_appender);
}

int main(int argc, char* argv[]) {
    setupLogging();

    PLOGI << "Setup AUX application";
    QApplication app(argc, argv);

    const auto services = boost::di::make_injector();

    PLOGI << "Startup AUX application";
    return app.exec();
}
