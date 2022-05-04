#ifndef IMESSAGESENDERSMANAGER_H
#define IMESSAGESENDERSMANAGER_H

// oss
#include <rxqt.h>

class IMessageSendersManager : public QObject {

public:
    virtual ~IMessageSendersManager() noexcept = default;

    virtual void StartWorkOn(const rxcpp::observe_on_one_worker& coordinator) = 0;

};

#endif // IMESSAGESENDERSMANAGER_H
