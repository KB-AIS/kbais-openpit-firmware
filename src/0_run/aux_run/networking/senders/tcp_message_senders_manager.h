#ifndef TCP_MESSAGE_SENDERS_MANAGER_H
#define TCP_MESSAGE_SENDERS_MANAGER_H

// qt
#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QSharedPointer>
#include <QTimer>
#include <QUuid>
// qt deferred
#include <QLambdaThreadWorker>

#include "networking/senders/base_message_senders_manager.h"
#include "networking/senders/message_sender.h"

class TcpMessageSendersManager : public BaseMessageSendersManager {
    Q_OBJECT

public:
    TcpMessageSendersManager();

    Q_SLOT void handleConfigurationChanged(
        const QList<MessageSenderConfiguration>& configurations
    ) override;

private:
    QHash<QUuid, MessageSender*> messageSenders;

    QHash<QUuid, QSharedPointer<BaseProtocolCommunicator>> protocolCommunicators;

    QHash<QUuid, MessageSenderConfiguration> messageSenderConfigurations;

    QHash<QUuid, MesssageSenderStatus> messageSenderStatuses;

    QLambdaThreadWorker threadWorker;

    QTimer restartMessageSendersTimer;

    QMutex mutex;

    /*!
     * Restart all registered instances of \ref MessageSender that have
     * TCP socket disconnected status.
     */
    void handleRestartMessageSenders();

    /*!
     * This slot reacts on socket state changing and store it as a currnet
     * status of \ref MesssageSender in instance of \ref MessageSenderStatus.
     */
    void handleMessageSenderStatusChanged(MessageSenderStatusChanged notification);

};

#endif // TCP_MESSAGE_SENDERS_MANAGER_H
