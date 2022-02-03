#ifndef SWOMPROTOCOLCOMMUNICATOR_H
#define SWOMPROTOCOLCOMMUNICATOR_H

#include "Networking/Communicators/IProtocolCommunicator.h"

class SwomProtocolCommunicator : public IProtocolCommunicator {

public:
    SwomProtocolCommunicator();

    void beging_communication(QIODevice &device);

    void finish_communication();
};

#endif // SWOMPROTOCOLCOMMUNICATOR_H
