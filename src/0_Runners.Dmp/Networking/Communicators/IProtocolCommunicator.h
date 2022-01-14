#ifndef IPROTOCOLCOMMUNICATOR_H
#define IPROTOCOLCOMMUNICATOR_H

// qt
#include <QByteArray>
#include <QIODevice>
#include <QObject>

class IProtocolCommunicator : public QObject {

public:
    virtual ~IProtocolCommunicator() noexcept = default;

    virtual void beginCommunication(QIODevice& device) = 0;

    virtual void endCommunication() = 0;

    virtual void sendNow() = 0;

};

#endif // IPROTOCOLCOMMUNICATOR_H
