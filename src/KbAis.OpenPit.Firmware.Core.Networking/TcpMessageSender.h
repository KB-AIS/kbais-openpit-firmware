#ifndef TCPMESSAGESENDER_H
#define TCPMESSAGESENDER_H

// qt
#include <QObject>
#include <QSharedPointer>
#include <QTcpSocket>

#include "IProtocolCommunicator.h"
#include "MessageSenderConfiguration.h"

class TcpMessageSender : public QObject {
    Q_OBJECT

    using SocketState = QAbstractSocket::SocketState;

    using SocketError = QAbstractSocket::SocketError;

public:
    TcpMessageSender(const QString& message_sender_name);

    void restart(const MessageSenderConfiguration& configuration);

private:
    const QString m_message_sender_name;

    QTcpSocket m_socket;

    void setup_socket_signals();

};

#endif // TCPMESSAGESENDER_H
