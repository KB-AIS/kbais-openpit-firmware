#ifndef MODULEBOOTSTRAPERSERIALDEVICES_H
#define MODULEBOOTSTRAPERSERIALDEVICES_H

#include "Modules/Sensors/Serials/RxFuelMessagePublisher.h"
#include "Modules/Sensors/Serials/SerialRxLlsSensorPublisher.h"
#include "Modules/Sensors/Serials/TkCardReader/SerialRxRcrSensorPublisher.h"
#include "RxQt.h"

class ModuleBootstraperSerialDevices {

    RxServiceCardReader& m_service_card_reader;

    SerialRxLlsSensorPublisher& m_lls_message_publisher;

    RxFuelMessagePublisher& m_fuel_message_publisher;

    QThread m_thrWorker;

    std::unique_ptr<rxqt::run_loop> m_runLoop;

public:
    ModuleBootstraperSerialDevices(
        RxServiceCardReader& service_card_reader
    ,   SerialRxLlsSensorPublisher& llsSensorPublisher
    ,   RxFuelMessagePublisher& fuel_message_publisher
    );

};

#endif // MODULEBOOTSTRAPERSERIALDEVICES_H
