#ifndef BASE_MESSAGE_SENDERS_MANAGER_H
#define BASE_MESSAGE_SENDERS_MANAGER_H

// qt
#include <QObject>

#include "networking/senders/message_sender.h"

class BaseMessageSendersManager : public QObject {
    Q_OBJECT

public:
    virtual ~BaseMessageSendersManager() noexcept = default;

    Q_SLOT virtual void handleConfigurationChanged(const QList<MessageSenderConfiguration>&) = 0;

};

#endif // BASE_MESSAGE_SENDERS_MANAGER_H
