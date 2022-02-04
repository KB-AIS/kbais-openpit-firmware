#include "AgtpThreadWorker.h"

using namespace std::chrono;

template <class F>
void waitUnitl(F f, milliseconds timeout = milliseconds(1000)) {
    QTimer timer;
    timer.setSingleShot(true);
    timer.setTimerType(Qt::PreciseTimer);
    timer.start(timeout.count());

    while (!f() && timer.remainingTime() >= 0) {
        qApp->processEvents();
    }
}

AgtpThreadWorker::AgtpThreadWorker(
    IAgtpRequetsReciever& requestsReciever
,   DeviceStateCollector& deviceStateCollector
)
    :   mRequestsReciever { requestsReciever }
    ,   mDeviceStateCollector { deviceStateCollector }
{
    mWorker.setObjectName("MODULES.LEGACY.AGTP");

    mRequestsReciever.moveToThread(&mWorker);

    QObject::connect(
        &mWorker, &QThread::started
    ,   &mRequestsReciever, &IAgtpRequetsReciever::start
    );

    QObject::connect(&mWorker, &QThread::finished, &mWorker, &QObject::deleteLater);

    mWorker.start();

    waitUnitl([&]() { return mWorker.runLoop() != nullptr; });

    mDeviceStateCollector.start(*mWorker.runLoop());
}
