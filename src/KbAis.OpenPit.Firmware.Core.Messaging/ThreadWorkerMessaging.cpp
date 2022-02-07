#include "ThreadWorkerMessaging.h"

#include "QtExtensions/QTimerExt.h"

ThreadWorkerMessaging::ThreadWorkerMessaging(
    MessagesCollectorsAdapter& messagesCollectorsAdapter
) {
    m_workingThread.setObjectName("CORE.MESSAGING");
    messagesCollectorsAdapter.moveToThread(&m_workingThread);

    m_workingThread.start();

    wait_using_timer([&]() { return m_workingThread.runLoop() != nullptr; });

    const auto scheduler = m_workingThread.runLoop()->observe_on_run_loop();

    messagesCollectorsAdapter.StartCollectingOn(scheduler);
}
