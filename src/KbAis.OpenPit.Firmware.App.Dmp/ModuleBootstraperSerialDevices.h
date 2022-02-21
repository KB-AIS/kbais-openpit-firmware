#ifndef MODULEBOOTSTRAPERSERIALDEVICES_H
#define MODULEBOOTSTRAPERSERIALDEVICES_H

#include "SerialRxLlsSensorPublisher.h"

class ModuleBootstraperSerialDevices {

public:
    ModuleBootstraperSerialDevices(SerialRxLlsSensorPublisher& llsSensorPublisher);

private:
    SerialRxLlsSensorPublisher& m_llsSensorPublisher;

    QThread m_thrWorker;

    std::unique_ptr<rxqt::run_loop> m_runLoop;

};

#endif // MODULEBOOTSTRAPERSERIALDEVICES_H
