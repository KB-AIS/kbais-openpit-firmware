#ifndef IPROTOCOLCOMMUNICATOR_H
#define IPROTOCOLCOMMUNICATOR_H

// qt
#include <QIODevice>

class IProtocolCommunicator {

public:
    virtual ~IProtocolCommunicator() noexcept = default;

    virtual void InitCommunication(QIODevice& device) = 0;

    virtual void StopCommunication() = 0;

};

#endif // IPROTOCOLCOMMUNICATOR_H
