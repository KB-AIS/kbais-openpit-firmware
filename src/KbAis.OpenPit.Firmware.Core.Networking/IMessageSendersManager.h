#ifndef IMESSAGESENDERSMANAGER_H
#define IMESSAGESENDERSMANAGER_H

#include "MessageSenderConfiguration.h"

class IMessageSendersManager {

public:
    virtual ~IMessageSendersManager() noexcept = default;

    virtual void applyConfiguration(const MessageSenderConfiguration& configuration) = 0;

};

#endif // IMESSAGESENDERSMANAGER_H
