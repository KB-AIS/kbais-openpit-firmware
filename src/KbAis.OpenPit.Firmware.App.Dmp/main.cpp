// qt
#include <QApplication>
// oss
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>

// Core.Persisting
#include "Configuration/DatabaseConfigurator.h"
// Utils.TrdParty.BoostDi
#include "BoostDiExtensions.h"

#include "InjectorFactory.h"

void
configureLogging() {
    using namespace plog;

    static ConsoleAppender<TxtFormatter> consoleAppender;

    init(verbose).addAppender(&consoleAppender);
}

int main(int argc, char* argv[]) {
    configureLogging();

    PLOGI << "Setup DMP application";
    QApplication app(argc, argv);

    DatabaseConfigurator::configure();

    auto injector = InjectorFactory::create();
    eagerSingletons(injector);

    PLOGI << "Startup DMP application";
    return app.exec();
}
