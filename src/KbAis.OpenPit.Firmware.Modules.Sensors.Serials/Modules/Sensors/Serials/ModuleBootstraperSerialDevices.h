#ifndef MODULEBOOTSTRAPERSERIALDEVICES_H
#define MODULEBOOTSTRAPERSERIALDEVICES_H

#include "Modules/Sensors/Serials/RxFuelMessagePublisher.h"
#include "Modules/Sensors/Serials/SerialRxLlsSensorPublisher.h"
#include "RxQt.h"

class ModuleBootstraperSerialDevices {

public:
    ModuleBootstraperSerialDevices(
        SerialRxLlsSensorPublisher& llsSensorPublisher
    ,   const RxFuelMessagePublisher& fuel_message_publisher
    );

private:
    SerialRxLlsSensorPublisher& m_lls_message_publisher;

    const RxFuelMessagePublisher& m_fuel_message_publisher;

    QThread m_thrWorker;

    std::unique_ptr<rxqt::run_loop> m_runLoop;

};

#endif // MODULEBOOTSTRAPERSERIALDEVICES_H
