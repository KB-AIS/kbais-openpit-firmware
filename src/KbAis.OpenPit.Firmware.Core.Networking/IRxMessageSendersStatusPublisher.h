#ifndef IRXMESSAGESENDERSSTATUSPUBLISHER_H
#define IRXMESSAGESENDERSSTATUSPUBLISHER_H

// qt
#include <QVector>
// oss
#include <rxcpp/rx.hpp>

#include "MessageSenderStatus.h"

class IRxMessageSendersStatusPublisher {

public:
    virtual ~IRxMessageSendersStatusPublisher() noexcept = default;

    virtual rxcpp::observable<QVector<MessageSenderStatus>> getObservable() const = 0;

};

#endif // IRXMESSAGESENDERSSTATUSPUBLISHER_H
