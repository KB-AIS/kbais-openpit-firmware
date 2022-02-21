#include "ModuleBootstraperSerialDevices.h"

#include "QtExtensions/QTimerExt.h"

ModuleBootstraperSerialDevices::ModuleBootstraperSerialDevices(
    SerialRxLlsSensorPublisher& llsSensorPublisher
)
    :   m_llsSensorPublisher { llsSensorPublisher }
{
    m_thrWorker.setObjectName("MODULES.SENSORS.SERIAL");

    m_llsSensorPublisher.moveToThread(&m_thrWorker);

    QObject::connect(&m_thrWorker, &QThread::started, [&]() {
        m_runLoop = std::make_unique<rxqt::run_loop>();

        const auto coordinator = m_runLoop->observe_on_run_loop();

        m_llsSensorPublisher.StartPublishOn(coordinator);
    });

    QObject::connect(&m_thrWorker, &QThread::finished, &m_thrWorker, &QThread::deleteLater);

    m_thrWorker.start();
}
