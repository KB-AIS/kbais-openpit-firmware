#ifndef TCPMESSAGESENDER_H
#define TCPMESSAGESENDER_H

// qt
#include <QObject>
#include <QSharedPointer>
#include <QTcpSocket>

#include "IProtocolCommunicator.h"
#include "MessageSenderConfiguration.h"

struct TcpMessageSenderState {

    QAbstractSocket::SocketState socket_state { QAbstractSocket::UnconnectedState };

    QAbstractSocket::SocketError socket_error { QAbstractSocket::UnknownSocketError };

};

struct TcpMessageSenderStateChanged {

    QString message_sender_name;

    TcpMessageSenderState state;

};

Q_DECLARE_METATYPE(TcpMessageSenderStateChanged);

class TcpMessageSender : public QObject {
    Q_OBJECT

public:
    TcpMessageSender(const QString& message_sender_name);

    void restart(const MessageSenderConfiguration& configuration);

    Q_SIGNAL void state_changed(TcpMessageSenderStateChanged notification);

private:
    const QString m_message_sender_name;

    QTcpSocket m_socket;

    void setup_socket_signals();

};

#endif // TCPMESSAGESENDER_H
