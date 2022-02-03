#ifndef IPROTOCOLCOMMUNICATOR_H
#define IPROTOCOLCOMMUNICATOR_H

// qt
#include <QIODevice>

class IProtocolCommunicator {

public:
    virtual ~IProtocolCommunicator() noexcept = default;

    virtual void beging_communication(QIODevice& device) = 0;

    virtual void finish_communication() = 0;

};

#endif // IPROTOCOLCOMMUNICATOR_H
