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

#include "networking/base_protocol_formatter.h"
#include "networking/message_sender.h"

namespace KbAis::Cfw::Networking {

class MessageSendersManager : public QObject, public GitlModule {
    Q_OBJECT

public:
    MessageSendersManager();

    Q_SLOT void handleConfigurationChanged(
        const QList<MessageSenderConfiguration>& configurations
    );

    Q_SIGNAL void foo();

private:
    QMutex lock;
    bool needSend;

    QList<MessageSenderConfiguration> _configs;

    QHash<QUuid, MessageSender*> _messageSenders;

    QHash<QUuid, MessageSenderConfiguration> _messageSenderConfigurations;

    QHash<QUuid, MesssageSenderStatus> _messageSenderStatuses;

    QTimer _restartMessageSendersTimer;

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

} // kbais::cfw::networking

#endif // MESSAGE_SENDERS_MANAGER_H
