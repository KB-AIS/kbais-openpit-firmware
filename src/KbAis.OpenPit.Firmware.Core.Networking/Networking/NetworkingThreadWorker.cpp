#include "NetworkingThreadWorker.h"

#include "QtExtensions/QTimerExt.h"

using namespace std::chrono;

NetworkingThreadWorker::NetworkingThreadWorker(
    TcpMessageSendersManager& manager
)
    :   m_manager(manager)
{
    m_working_thread.setObjectName("CORE.NETWORKING");

    m_manager.moveToThread(&m_working_thread);

    QObject::connect(
        &m_working_thread, &QThread::finished
    ,   &m_working_thread, &QObject::deleteLater
    );

    m_working_thread.start();

    wait_using_timer([&]() {
        return m_working_thread.runLoop() != nullptr;
    });

    auto scheduler = m_working_thread.runLoop()->observe_on_run_loop();
    m_manager.StartWorkOn(scheduler);
}
