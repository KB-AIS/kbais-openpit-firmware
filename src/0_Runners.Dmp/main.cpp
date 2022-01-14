// qt
#include <QApplication>
#include <QMetaType>

// cfw::trdparty
#include "RxQt/RxQt.h"

#include "Persisting/Configuration/DatabaseConfigurator.h"
#include "Utils/BoostDiExtensions.h"
#include "CompositionRootFactory.h"
#include "LoggerConfigurator.h"

int main(int argc, char* argv[]) {
    LoggerConfigurator::configure();

    PLOGI << "Setup DMP application";
    QApplication app(argc, argv);

    rxqt::run_loop rxQtRunLoop;

    DatabaseConfigurator::configure();

    eagerSingletons(CompositionRootFactory::create());

    PLOGI << "Startup DMP application";
    return app.exec();
}
