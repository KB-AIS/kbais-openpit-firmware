// std
#include <memory>
// qt
#include <QApplication>
#include <QMetaType>

// cfw::inter::views::dmp
#include "ViewWrapper.h"
// cfw::trdparty
#include "RxQt/RxQt.h"

#include "AgtpCommandsMediator.h"
#include "AgtpUsbCommandsReciever.h"
#include "DeviceStateCollector.h"

#include "CompositionRootFactory.h"
#include "LoggerConfigurator.h"
#include "Persisting/Configuration/DatabaseConfigurator.h"
#include "Utils/BoostDiExtensions.h"

using namespace std::chrono;

template <class F>
void wait_until(F f, milliseconds timeout = milliseconds(1000))
{
    QTimer timer;
    timer.setSingleShot(true);
    timer.setTimerType(Qt::PreciseTimer);
    timer.start(timeout.count());
    while (!f() && timer.remainingTime() >= 0) {
        qApp->processEvents();
    }
}

class ThreadWrapper {

public:
    ThreadWrapper(
        AgtpUsbCommandsReciever* agtpService
    ,   DeviceStateCollector& deviceStateCollector
    )
        : mAgtpService { agtpService }
        , mAgtpServiceTrd { }
    {
        mAgtpServiceTrd.setObjectName("AgtpService");
        mAgtpService->moveToThread(&mAgtpServiceTrd);
        deviceStateCollector.moveToThread(&mAgtpServiceTrd);

        QObject::connect(
            &mAgtpServiceTrd, &QThread::started,
            mAgtpService, &AgtpUsbCommandsReciever::startProcessing);

        QObject::connect(
            &mAgtpServiceTrd, &QThread::finished,
            &mAgtpServiceTrd, &QObject::deleteLater);

        QObject::connect(
            mAgtpService, &AgtpUsbCommandsReciever::processingFinished,
            &mAgtpServiceTrd, &QThread::quit);

        QObject::connect(
            mAgtpService, &AgtpUsbCommandsReciever::processingFinished,
            mAgtpService, &QObject::deleteLater);

        mAgtpServiceTrd.start();

        wait_until([&]() { return mAgtpServiceTrd.runLoop() != nullptr; });

        deviceStateCollector.startCollecting(*mAgtpServiceTrd.runLoop());
    }

private:
    AgtpUsbCommandsReciever* mAgtpService;

    rxqt::RunLoopThread mAgtpServiceTrd;

};

int main(int argc, char* argv[]) {
    LoggerConfigurator::configure();

    PLOGI << "Setup DMP application";
    QApplication app(argc, argv);

    DatabaseConfigurator::configure();

    auto injector = CompositionRootFactory::create();
    eagerSingletons(injector);

    injector.create<std::shared_ptr<ThreadWrapper>>();

    PLOGI << "Startup DMP application";
    return app.exec();
}
