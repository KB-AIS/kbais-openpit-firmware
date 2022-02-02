#ifndef TCPMESSAGESENDER_H
#define TCPMESSAGESENDER_H

// std
#include <chrono>
#include <memory>
// qt
#include <QAbstractSocket>
#include <QObject>
#include <QSharedPointer>
#include <QTcpSocket>
#include <QTimer>
#include <QUuid>

#include "IProtocolCommunicator.h"
#include "MessageSenderConfiguration.h"

class TcpMessageSender : public QObject {
    Q_OBJECT

    using SocketState = QAbstractSocket::SocketState;

    using SocketError = QAbstractSocket::SocketError;

public:
    TcpMessageSender();

    void restart(const MessageSenderConfiguration& configuration);

private:
    const QString mMessageSenderName;

    QTcpSocket mSocket;

    void setupSocketSignals();

};

#endif // TCPMESSAGESENDER_H
