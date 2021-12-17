#ifndef MESSAGE_SENDER_H
#define MESSAGE_SENDER_H

// std
#include <chrono>
#include <memory>
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
#include "networking/base_protocol_communicator.h"

using SocketState = QAbstractSocket::SocketState;

using SocketError = QAbstractSocket::SocketError;

namespace KbAis::Cfw::Networking {

struct MesssageSenderStatus {

    SocketState state;

    SocketError error;

};

struct MessageSenderStatusChanged {

    QUuid senderId;

    SocketState lastState;

    SocketError lastError;

};

struct MessageSenderConfiguration {

    QString host;

    quint16 port;

    std::chrono::milliseconds dataSendInterval;

    QSharedPointer<BaseProtocolCommunicator> communicator;

};

class MessageSender : public QObject {
    Q_OBJECT

public:
    const QUuid id { QUuid::createUuid() };

    explicit MessageSender();

    void restart(const MessageSenderConfiguration& configuration);

    void sendMessage();

    Q_SIGNAL void notifyStatusChanged(MessageSenderStatusChanged notification);

private:
    QSharedPointer<BaseProtocolCommunicator> communicator;

    QTcpSocket socket;

    void connectSocketSignals();

};

} // kbais::cfw::networking

Q_DECLARE_METATYPE(KbAis::Cfw::Networking::MessageSenderStatusChanged);

#endif // MESSAGE_SENDER_H
