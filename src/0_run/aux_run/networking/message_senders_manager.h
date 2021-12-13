#ifndef MESSAGESENDERSMANAGER_H
#define MESSAGESENDERSMANAGER_H

#include "networking/base_protocol_formatter.h"
#include "networking/message_sender_client.h"
#include "networking/message_sender_params.h"
// qt
#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>
#include <QObject>
#include <QUuid>

namespace kbais::cfw::networking {

class MessageSendersManager : public QObject{
    Q_OBJECT

public:
    explicit MessageSendersManager(QObject* parent = nullptr);

    Q_SLOT void handleConfiguratingChanged();

private:
    QHash<QUuid, MessageSender*> _senders;

    std::vector<MessageSenderConfiguration> _senderConfigurations;

    void registerSender(const MessageSenderConfiguration& params);

    void handleSenderStateChanged(QUuid id, SocketState state);

};

} // kbais::cfw::networking

#endif // MESSAGESENDERSMANAGER_H
