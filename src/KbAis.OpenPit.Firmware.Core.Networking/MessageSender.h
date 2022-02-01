#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H

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

#include "Networking/Communicators/IProtocolCommunicator.h"


class MessageSender : public QObject {
    Q_OBJECT

public:
    const QUuid id { QUuid::createUuid() };

    MessageSender(QSharedPointer<IProtocolCommunicator> communicator);

    void restart(const MessageSenderConfiguration& configuration);

    void sendMessage();

    Q_SIGNAL void notifyStatusChanged(MessageSenderStatusChanged notification);

private:
    QTcpSocket socket;

    QSharedPointer<IProtocolCommunicator> communicator;

    void connectSocketSignals();

};

Q_DECLARE_METATYPE(MessageSenderStatusChanged);

#endif // MESSAGESENDER_H
