// std
#include <memory>
// qt
#include <QApplication>
#include <QMetaType>

// cfw::inter::views::dmp
#include "ViewWrapper.h"
// cfw::trdparty
#include "RxQt/RxQt.h"

#include "AgtpUsbCommandsReciever.h"
#include "AgtpCommandsMediator.h"
#include "CompositionRootFactory.h"
#include "LoggerConfigurator.h"
#include "Persisting/Configuration/DatabaseConfigurator.h"
#include "Utils/BoostDiExtensions.h"

class ThreadWrapper {

AgtpUsbCommandsReciever* mAgtpService;

std::unique_ptr<rxqt::RunLoopThread> mAgtpServiceTrd;

public:
    ThreadWrapper(AgtpUsbCommandsReciever* agtpService)
        : mAgtpService { agtpService }
        , mAgtpServiceTrd { std::make_unique<rxqt::RunLoopThread>() }
    {
        mAgtpService->moveToThread(mAgtpServiceTrd.get());

        QObject::connect(
            mAgtpServiceTrd.get(), &QThread::started,
            mAgtpService, &AgtpUsbCommandsReciever::startProcessing);

        QObject::connect(
            mAgtpServiceTrd.get(), &QThread::finished,
            mAgtpServiceTrd.get(), &QObject::deleteLater);

        QObject::connect(
            mAgtpService, &AgtpUsbCommandsReciever::processingFinished,
            mAgtpServiceTrd.get(), &QThread::quit);

        QObject::connect(
            mAgtpService, &AgtpUsbCommandsReciever::processingFinished,
            mAgtpService, &QObject::deleteLater);

        mAgtpServiceTrd->start();
    }

    ThreadWrapper(const ThreadWrapper&) = default;

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
