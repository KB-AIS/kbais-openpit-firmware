#include "ModuleBootstraperSerialDevices.h"

#include "QtExtensions/QTimerExt.h"

ModuleBootstraperSerialDevices::ModuleBootstraperSerialDevices(
    SerialRxLlsSensorPublisher& lls_message_publisher
,   RxFuelMessagePublisher& fuel_message_publisher
)
    :   m_lls_message_publisher(lls_message_publisher)
    ,   m_fuel_message_publisher(fuel_message_publisher)
{
    m_thrWorker.setObjectName("MODULES.SENSORS.SERIAL");

    m_lls_message_publisher.moveToThread(&m_thrWorker);

    QObject::connect(&m_thrWorker, &QThread::started, [&]() {
        m_runLoop = std::make_unique<rxqt::run_loop>();

        const auto coordination = m_runLoop->observe_on_run_loop();

        m_lls_message_publisher.start_publish_on(coordination);
        m_fuel_message_publisher.start_publish_on(coordination);
    });

    QObject::connect(&m_thrWorker, &QThread::finished, &m_thrWorker, &QThread::deleteLater);

    m_thrWorker.start();
}
