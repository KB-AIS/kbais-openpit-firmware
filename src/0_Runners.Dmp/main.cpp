// qt
#include <QApplication>
#include <QMetaType>

// cfw::inter::views::dmp
#include "ViewWrapper.h"
// cfw::trdparty
#include "RxQt/RxQt.h"

#include "UsbAgtpCommandsReciever.h"
#include "CompositionRootFactory.h"
#include "LoggerConfigurator.h"
#include "Persisting/Configuration/DatabaseConfigurator.h"
#include "Utils/BoostDiExtensions.h"

int main(int argc, char* argv[]) {
    LoggerConfigurator::configure();

    PLOGI << "Setup DMP application";
    QApplication app(argc, argv);

    rxqt::run_loop mainRunLoop;

    // DatabaseConfigurator::configure();

    // eagerSingletons(CompositionRootFactory::create());

    auto trdAgtpService = new rxqt::RunLoopThread();

    UsbAgtpCommandsReciever agtpService;
    agtpService.moveToThread(trdAgtpService);

    QObject::connect(trdAgtpService, &QThread::started, &agtpService, &UsbAgtpCommandsReciever::startProcessing);
    QObject::connect(trdAgtpService, &QThread::finished, trdAgtpService, &QObject::deleteLater);

    QObject::connect(&agtpService, &UsbAgtpCommandsReciever::processingFinished, trdAgtpService, &QThread::quit);
    QObject::connect(&agtpService, &UsbAgtpCommandsReciever::processingFinished, &agtpService, &QObject::deleteLater);

    trdAgtpService->start();

    PLOGI << "Startup DMP application";
    return app.exec();
}
