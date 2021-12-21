#ifndef BASE_PROTOCOL_COMMUNICATOR_H
#define BASE_PROTOCOL_COMMUNICATOR_H

// qt
#include <QByteArray>
#include <QIODevice>
#include <QObject>

class BaseProtocolCommunicator : public QObject {
    Q_OBJECT

public:
    virtual void start(QIODevice& device) = 0;

    virtual void cease() = 0;

    virtual void sendMessage() = 0;

    Q_SIGNAL void notifyNeedSend();

};

#endif // BASE_PROTOCOL_COMMUNICATOR_H
