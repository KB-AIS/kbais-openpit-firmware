#ifndef MESSAGESENDERSMANAGER_H
#define MESSAGESENDERSMANAGER_H

#include "networking/base_protocol_formatter.h"
#include "networking/message_sender.h"
#include "networking/message_sender_params.h"
// qt
#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>
#include <QObject>
#include <QTimer>
#include <QUuid>

namespace kbais::cfw::networking {

struct MesssageSenderStatus {

    SocketState lastState;

    SocketError lastError;

};

class MessageSendersManager : public QObject{
    Q_OBJECT

public:
    explicit MessageSendersManager(QObject* parent = nullptr);

    Q_SLOT void handleConfiguratingChanged(
        const QList<MessageSenderConfiguration>& configurations
    );

private:
    QHash<QUuid, MessageSender*> _messageSenders;

    QHash<QUuid, MessageSenderConfiguration> _messageSenderConfigurations;

    QHash<QUuid, MesssageSenderStatus> _messageSenderStatuses;

    QTimer _restartMessageSendersTimer;

    void restartMessageSenders();

    Q_SLOT void handleMessageSenderStatusChanged(
        QUuid id, SocketState lastState, SocketError lastError
    );

};

} // kbais::cfw::networking

#endif // MESSAGESENDERSMANAGER_H
