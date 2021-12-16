#ifndef BASE_PROTOCOL_COMMUNICATOR_H
#define BASE_PROTOCOL_COMMUNICATOR_H

// qt
#include <QByteArray>
#include <QIODevice>
#include <QObject>

namespace KbAis::Cfw::Networking {

class BaseProtocolCommunicator : public QObject {
    Q_OBJECT

public:
    virtual void start(QIODevice& device) = 0;

    virtual void cease() = 0;

    Q_SIGNAL void notifyDisconnectRequired();

};

}

#endif // BASE_PROTOCOL_COMMUNICATOR_H
