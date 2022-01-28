#ifndef IMESSAGESENDERSMANAGER_H
#define IMESSAGESENDERSMANAGER_H

// qt
#include <QObject>

#include "Networking/Senders/MessageSender.h"

class IMessageSendersManager : public QObject {
    Q_OBJECT

public:
    virtual ~IMessageSendersManager() noexcept = default;

    Q_SLOT virtual void handleConfigurationChanged(const QList<MessageSenderConfiguration>&) = 0;

};

#endif // IMESSAGESENDERSMANAGER_H
