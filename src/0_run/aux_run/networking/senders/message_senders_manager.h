#ifndef MESSAGE_SENDERS_MANAGER_H
#define MESSAGE_SENDERS_MANAGER_H

// qt
#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>
#include <QObject>
#include <QTimer>
#include <QUuid>
// oss
#include <gitlmodule.h>
#include <gitleventbus.h>

#include "networking/senders/base_message_senders_manager.h"
#include "networking/senders/message_sender.h"

class TcpMessageSendersManager : public BaseMessageSendersManager, public GitlModule {
    Q_OBJECT

public:
    TcpMessageSendersManager();

    Q_SLOT void handleConfigurationChanged(
        const QList<MessageSenderConfiguration>& configurations
    ) override;

private:
    QHash<QUuid, MessageSender*> messageSenders;

    QHash<QUuid, MessageSenderConfiguration> messageSenderConfigurations;

    QHash<QUuid, MesssageSenderStatus> messageSenderStatuses;

    QTimer restartMessageSendersTimer;

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

#endif // MESSAGE_SENDERS_MANAGER_H
