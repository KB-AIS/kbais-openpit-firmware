#ifndef IMMEDIATEMESSAGESCOLLECTOR_H
#define IMMEDIATEMESSAGESCOLLECTOR_H

// qt
#include <QMutex>
#include <QVector>

// Utils.TrdParty.RxQt
#include "RxQt.h"

#include "IRxImmediateMessageMapper.h"
#include "Message.h"

class ImmediateMessagesCollector {
    using ImmediateMessageMappers_t = std::vector<std::shared_ptr<IRxImmediateMessageMapper>>;

public:
    ImmediateMessagesCollector(ImmediateMessageMappers_t mappers);

    ~ImmediateMessagesCollector();

    void startCollectingOn(const rxqt::run_loop& loop);

    rxcpp::observable<long> getCollectedObservable();

    QVector<Message> dumpMessages();

private:
    rxcpp::observable<Message> mObservable;

    rxcpp::composite_subscription mSubscriptions;

    rxcpp::rxsub::subject<long> mSubject;

    QVector<Message> mMessages;

    QMutex mMtxMessages;

};

#endif // IMMEDIATEMESSAGESCOLLECTOR_H
