// qt
#include <QApplication>

// Utils.TrdParty.BoostDi
#include "BoostDiExtensions.h"

#include "InjectorFactory.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    auto injector = InjectorFactory::create();
    eagerSingletons(injector);

    return app.exec();
}
