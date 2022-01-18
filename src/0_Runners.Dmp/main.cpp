// qt
#include <QApplication>
#include <QMetaType>

// cfw::inter::views::dmp
#include "ViewWrapper.h"
// cfw::trdparty
#include "RxQt/RxQt.h"

#include "Persisting/Configuration/DatabaseConfigurator.h"
#include "Utils/BoostDiExtensions.h"
#include "CompositionRootFactory.h"
#include "LoggerConfigurator.h"
#include "LegacyConfigService.h"

int main(int argc, char* argv[]) {
    LoggerConfigurator::configure();

    PLOGI << "Setup DMP application";
    QApplication app(argc, argv);

    rxqt::run_loop mainRunLoop;

    // DatabaseConfigurator::configure();

    // eagerSingletons(CompositionRootFactory::create());

    auto trdAgtpTerminal = new rxqt::RunLoopThread();

    LegacyConfigService usbDeviceService;
    usbDeviceService.moveToThread(trdAgtpTerminal);

    QObject::connect(trdAgtpTerminal, &QThread::started, &usbDeviceService, &LegacyConfigService::start);
    QObject::connect(&usbDeviceService, &LegacyConfigService::finished, trdAgtpTerminal, &QThread::quit);

    QObject::connect(&usbDeviceService, &LegacyConfigService::finished, &usbDeviceService, &QObject::deleteLater);
    QObject::connect(trdAgtpTerminal, &QThread::finished, trdAgtpTerminal, &QObject::deleteLater);

    trdAgtpTerminal->start();

    PLOGI << "Startup DMP application";
    return app.exec();
}
