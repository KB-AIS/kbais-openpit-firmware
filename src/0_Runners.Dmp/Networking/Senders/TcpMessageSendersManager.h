#ifndef TCPMESSAGESENDERSMANAGER_H
#define TCPMESSAGESENDERSMANAGER_H

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

// cfw::infra::eventbus
#include "RxEventBus.h"

#include "Networking/Senders/IMessageSendersManager.h"
#include "Networking/Senders/MessageSender.h"

class TcpMessageSendersManager : public IMessageSendersManager, public RxEventModule {
    Q_OBJECT

public:
    TcpMessageSendersManager(const RxEventBus& eventBus);

    Q_SLOT void handleConfigurationChanged(
        const QList<MessageSenderConfiguration>& configurations
    ) override;

private:
    QHash<QUuid, MessageSender*> messageSenders;

    QHash<QUuid, QSharedPointer<IProtocolCommunicator>> protocolCommunicators;

    QHash<QUuid, MessageSenderConfiguration> messageSenderConfigurations;

    QHash<QUuid, MesssageSenderStatus> messageSenderStatuses;

    QTimer timerRestartMessageSenders;

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

#endif // TCPMESSAGESENDERSMANAGER_H
