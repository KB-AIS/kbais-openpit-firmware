#include "ThreadWorkerMessaging.h"

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

ThreadWorkerMessaging::ThreadWorkerMessaging(
    ImmediateMessagesCollector& immediateMessagesCollector
,   RecurrentMessagesCollector& recurrentMessagesCollector
,   MessagesCollectorsAdapter& messagesCollectorsAdapter
) {
    messagesCollectorsAdapter.moveToThread(&mTrdWorker);

    mTrdWorker.start();

    waitUnitl([&]() { return mTrdWorker.runLoop() != nullptr; });

    immediateMessagesCollector.startCollectingOn(*mTrdWorker.runLoop());
    recurrentMessagesCollector.startCollectingOn(*mTrdWorker.runLoop());
}
