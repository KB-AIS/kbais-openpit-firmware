#include "CachingBootstrapper.h"

#include "QtExtensions/QTimerExt.h"

CachingBootstrapper::CachingBootstrapper(IMessagesCachingService& messagesCachingService) {
    m_workingThread.setObjectName("CORE.PERSISTING_CACHING");

    m_workingThread.start();

    wait_using_timer([&]() {
        return m_workingThread.runLoop() != nullptr;
    });

    auto coordination = m_workingThread.runLoop()->observe_on_run_loop();
    messagesCachingService.StartWorkOn(coordination);
}
