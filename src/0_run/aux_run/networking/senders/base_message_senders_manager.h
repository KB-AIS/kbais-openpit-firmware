#ifndef BASE_MESSAGE_SENDERS_MANAGER_H
#define BASE_MESSAGE_SENDERS_MANAGER_H

// qt
#include <QObject>

#include <networking/message_sender.h>

namespace KbAis::Cfw::Networking {

class BaseMessageSendersManager : public QObject {
    Q_OBJECT

public:
    Q_SLOT virtual void handleConfigurationChanged(const QList<MessageSenderConfiguration>&) = 0;

};

}

#endif // BASE_MESSAGE_SENDERS_MANAGER_H
