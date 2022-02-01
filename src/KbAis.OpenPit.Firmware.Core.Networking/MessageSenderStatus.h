#ifndef MESSAGESENDERSTATUS_H
#define MESSAGESENDERSTATUS_H

#include <QAbstractSocket>

struct MessageSenderStatus {
    using SocketState = QAbstractSocket::SocketState;

    using SocketError = QAbstractSocket::SocketError;

    QString messageSenderId;

    SocketState lastState;

    SocketError lastError;

};

#endif // MESSAGESENDERSTATUS_H
