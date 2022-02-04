#ifndef TCPMESSAGESENDER_H
#define TCPMESSAGESENDER_H

// std
#include <memory>
// qt
#include <QObject>
#include <QSharedPointer>
#include <QTcpSocket>

#include "Networking/Communicators/IProtocolCommunicator.h"
#include "Networking/Confguration/TcpMessageSenderConfiguration.h"

struct TcpMessageSenderState {

    QAbstractSocket::SocketState socket_state {
        QAbstractSocket::UnconnectedState
    };

    QAbstractSocket::SocketError socket_error {
        QAbstractSocket::UnknownSocketError
    };

};

struct TcpMessageSenderStateChanged {

    QString message_sender_name;

    TcpMessageSenderState state;

};

Q_DECLARE_METATYPE(TcpMessageSenderStateChanged);

class TcpMessageSender : public QObject {
    Q_OBJECT

    using ProtocolCommunicator_t = std::unique_ptr<IProtocolCommunicator>;

public:
    TcpMessageSender(const QString& messageSenderName);

    void Restart(const TcpMessageSenderConfiguration& configuration);

    Q_SIGNAL void StateChanged(TcpMessageSenderStateChanged notification);

private:
    const QString m_messageSenderName;

    QTcpSocket m_socket;

    ProtocolCommunicator_t m_protocolCommunicator;

    void SetupSocketSignals();

    static ProtocolCommunicator_t GenProtocolCommunicator(const MessageSenderProtocol protocol);

};

#endif // TCPMESSAGESENDER_H
