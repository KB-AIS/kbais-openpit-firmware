#include "Core/Networking/ModuleBootstraperNetworking.h"

#include "QtExtensions/QTimerExt.h"

using namespace std::chrono;

ModuleBootstraperNetworking::ModuleBootstraperNetworking(
    IMessageSendersManager& manager
)
    :   m_manager(manager)
{
    m_thrWorker.setObjectName("CORE.NETWORKING");

    m_manager.moveToThread(&m_thrWorker);

    QObject::connect(&m_thrWorker, &QThread::finished, &m_thrWorker, &QObject::deleteLater);

    m_thrWorker.start();

    wait_using_timer([&]() { return m_thrWorker.runLoop() != nullptr; });

    const auto coordinator = m_thrWorker.runLoop()->observe_on_run_loop();
    m_manager.StartWorkOn(coordinator);
}
