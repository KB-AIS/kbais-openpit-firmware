// std
#include <memory>
// qt
#include <QApplication>
#include <QMetaType>

// cfw::inter::views::dmp
#include "ViewWrapper.h"
// cfw::trdparty
#include "RxQt/RxQt.h"

#include "CompositionRootFactory.h"
#include "LoggerConfigurator.h"
#include "Persisting/Configuration/DatabaseConfigurator.h"
#include "Utils/BoostDiExtensions.h"

int main(int argc, char* argv[]) {
    LoggerConfigurator::configure();

    PLOGI << "Setup DMP application";
    QApplication app(argc, argv);

    DatabaseConfigurator::configure();

    auto injector = CompositionRootFactory::create();
    eagerSingletons(injector);

    PLOGI << "Startup DMP application";
    return app.exec();
}
