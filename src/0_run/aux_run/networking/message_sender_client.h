#ifndef MESSAGE_SENDER_CLIENT_H
#define MESSAGE_SENDER_CLIENT_H

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

struct Ack {
    QString status;
};

class MessageSender : public QObject {
    Q_OBJECT

public:
    const QUuid id { QUuid::createUuid() };

    explicit MessageSender(MessageSenderConfiguration configuration);

    void restart();

    Q_SIGNAL void notifyStateChanged(QUuid id, SocketState state);

private:
    MessageSenderConfiguration _configuration;

    msgcaching::GetMessageBatchesQueryHandler queryHandler;

    QTcpSocket _socket;

    QTimer _timerSendReqeust;

    QStateMachine _reducer;

    QState sIDLE;

    QState sCONNECTING;

    QState sCONNECTED;

    Q_SIGNAL void eCONNECT_REQUESTED();

//    QState stateAuthenticationRequested;

//    QState stateAuthenticated;

//    QState stateMessagesSended;
};

} // kbais::cfw::networking

#endif // MESSAGE_SENDER_CLIENT_H
