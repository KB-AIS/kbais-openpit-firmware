#ifndef MESSAGE_SENDERS_MANAGER_H
#define MESSAGE_SENDERS_MANAGER_H

// qt
#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>
#include <QObject>
#include <QTimer>
#include <QUuid>

#include "networking/base_protocol_formatter.h"
#include "networking/message_sender.h"

namespace kbais::cfw::networking {

class MessageSendersManager : public QObject{
    Q_OBJECT

public:
    explicit MessageSendersManager(QObject* parent = nullptr);

    Q_SLOT void handleConfigurationChanged(
        const QList<MessageSenderConfiguration>& configurations
    );

private:
    QHash<QUuid, MessageSender*> _messageSenders;

    QHash<QUuid, MessageSenderConfiguration> _messageSenderConfigurations;

    QHash<QUuid, MesssageSenderStatus> _messageSenderStatuses;

    QTimer _restartMessageSendersTimer;

    /*!
     * Restart all registered instances of \ref MessageSender that have
     * TCP socket disconnected status.
     */
    void restartMessageSenders();

    /*!
     * This slot reacts on socket state changing and store it as a currnet
     * status of \ref MesssageSender in instance of \ref MessageSenderStatus.
     * \param id
     *      An identifier of an instance of \ref MessageSender.
     * \param lastState
     *      A last state of TCP socket wrapped in \ref MessageSender.
     * \param lastError
     *      A last error produced by TCP socket wrapped in \ref MessageSender.
     */
    Q_SLOT void handleMessageSenderStatusChanged(
        QUuid senderId, SocketState lastState, SocketError lastError
    );

};

} // kbais::cfw::networking

#endif // MESSAGE_SENDERS_MANAGER_H
