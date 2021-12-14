#ifndef MESSAGE_SENDER_H
#define MESSAGE_SENDER_H

// std
#include <chrono>
// qt
#include <QAbstractSocket>
#include <QFinalState>
#include <QObject>
#include <QState>
#include <QStateMachine>
#include <QTcpSocket>
#include <QTimer>
#include <QUuid>

#include "msgcaching/get_message_batches_query.h"
#include "networking/base_protocol_formatter.h"

using SocketState = QAbstractSocket::SocketState;

using SocketError = QAbstractSocket::SocketError;

namespace kbais::cfw::networking {

struct MesssageSenderStatus {

    SocketState lastState;

    SocketError lastError;

};

struct MessageSenderConfiguration {

    QString host;

    quint16 port;

    std::chrono::milliseconds intervalSendMessages;

    BaseProtocolFormatter formatter;

};

class MessageSender : public QObject {
    Q_OBJECT

public:
    const QUuid id { QUuid::createUuid() };

    explicit MessageSender();

    void restart(MessageSenderConfiguration configuration);

    Q_SIGNAL void notifyStatusChanged(
        QUuid id, SocketState lastState, SocketError lastError
    );

private:
    QString host;

    quint16 port;

    QTcpSocket _socket;

    QTimer _timerSendMessages;

    QStateMachine _reducer;

    QMetaObject::Connection _cReducerRestart;

    QState stateIdle;

    QState stateConnecting;

    QState stateConnected;

    void connectSocketSignals();

    Q_SIGNAL void eventConnectRequested();
};

struct Ack {
    QString status;
};

} // kbais::cfw::networking

#endif // MESSAGE_SENDER_H
