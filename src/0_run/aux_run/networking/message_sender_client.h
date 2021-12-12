#ifndef MESSAGE_SENDER_CLIENT_H
#define MESSAGE_SENDER_CLIENT_H

#include "base_protocol_formatter.h"
#include "msgcaching/get_message_batches_query.h"
#include "networking/message_sender_params.h"
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
