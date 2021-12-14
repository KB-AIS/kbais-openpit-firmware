#ifndef MESSAGE_SENDER_H
#define MESSAGE_SENDER_H

#include "base_protocol_formatter.h"
#include "msgcaching/get_message_batches_query.h"
#include "networking/message_sender_params.h"
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
// oss
#include <json.hpp>

using json = nlohmann::json;

using SocketState = QAbstractSocket::SocketState;

using SocketError = QAbstractSocket::SocketError;

namespace kbais::cfw::networking {

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
    msgcaching::GetMessageBatchesQueryHandler queryHandler;

    QString host;

    quint16 port;

    QTcpSocket _socket;

    QTimer _timerSendReqeust;

    QStateMachine _reducer;

    QMetaObject::Connection _connectionReducerRestart;

    QState stateIdle;

    QState sCONNECTING;

    QState sCONNECTED;

    void restartReducer();

    Q_SIGNAL void eCONNECT_REQUESTED();
};

struct Ack {
    QString status;
};

} // kbais::cfw::networking

#endif // MESSAGE_SENDER_H
