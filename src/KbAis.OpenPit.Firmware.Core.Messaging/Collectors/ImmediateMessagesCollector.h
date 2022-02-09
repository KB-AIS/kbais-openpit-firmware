#ifndef IMMEDIATEMESSAGESCOLLECTOR_H
#define IMMEDIATEMESSAGESCOLLECTOR_H

// qt
#include <QVector>

// Utils.TrdParty.RxQt
#include "RxQt.h"

#include "IRxImmediateMessageMapper.h"
#include "Message.h"

class ImmediateMessagesCollector {
    using ImmediateMessageMappers_t = std::vector<std::shared_ptr<IRxImmediateMessageMapper>>;

    using Scheduler_t = rxcpp::observe_on_one_worker;

public:
    ImmediateMessagesCollector(ImmediateMessageMappers_t mappers);

    ~ImmediateMessagesCollector();

    void StartCollectingOn(const Scheduler_t& scheduler);

    rxcpp::observable<bool> GetMessagesCollectedObservable(const Scheduler_t& scheduler);

    QVector<Message> DumpMessages();

private:
    rxcpp::observable<Message> m_observableImmediateMappers;

    rxcpp::composite_subscription m_subs;

    rxcpp::rxsub::subject<bool> m_messagesCollectedSubject;

    QVector<Message> m_collectedMessages;

};

#endif // IMMEDIATEMESSAGESCOLLECTOR_H
