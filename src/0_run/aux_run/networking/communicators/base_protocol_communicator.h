#ifndef BASE_PROTOCOL_COMMUNICATOR_H
#define BASE_PROTOCOL_COMMUNICATOR_H

// qt
#include <QByteArray>
#include <QIODevice>
#include <QObject>

class BaseProtocolCommunicator : public QObject {
public:
    virtual ~BaseProtocolCommunicator() noexcept = default;

    virtual void beginCommunication(QIODevice& device) = 0;

    virtual void endCommunication() = 0;

    virtual void requestSendImmediatly() = 0;

};

#endif // BASE_PROTOCOL_COMMUNICATOR_H
