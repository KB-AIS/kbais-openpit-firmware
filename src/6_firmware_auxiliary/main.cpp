// Qt
#include <QApplication>
// Firmware AUX
#include <forms/main_form.h>
// OSS
#include <plog/Init.h>
#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>

static void setup_logging() {
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug).addAppender(&consoleAppender);
}

int main(int argc, char *argv[]) {
    setup_logging();

    PLOGI << "Starting AUX application";

    QApplication app(argc, argv);

    main_form main_form;
    main_form.show();

    return app.exec();
}
