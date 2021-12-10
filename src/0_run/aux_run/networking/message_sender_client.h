#ifndef MESSAGE_SENDER_CLIENT_H
#define MESSAGE_SENDER_CLIENT_H

#include "msgcaching/get_message_batches_query.h"
#include "base_protocol_formatter.h"
// std
#include <chrono>
// Qt
#include <QAbstractSocket>
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QUuid>

using SocketState = QAbstractSocket::SocketState;

using SocketError = QAbstractSocket::SocketError;

namespace kbais::cfw::networking {

struct MessageSenderParams {

    const QHostAddress& host;

    const quint16 port;

    const std::chrono::milliseconds requestInterval;

    const BaseProtocolFormatter& fmt;

};

class MessageSenderClient : public QObject {
    Q_OBJECT

public:
    const QUuid id { QUuid::createUuid() };

    explicit MessageSenderClient(QObject* parent = nullptr);

    void restart(const MessageSenderParams& parms);

    Q_SIGNAL void notifyStateChanged(SocketState state);

private:
    QTcpSocket socket;

    QTimer sendMessagesTimer;

    msgcaching::GetMessageBatchesQueryHandler getMessageBatchesQueryHandler;

};

} // kbais::cfw::networking

#endif // MESSAGE_SENDER_CLIENT_H
