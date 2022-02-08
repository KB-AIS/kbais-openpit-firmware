#ifndef RECURRENTMESSAGESCOLLECTOR_H
#define RECURRENTMESSAGESCOLLECTOR_H

// qt
#include <QHash>
#include <QMutex>
#include <QVector>

// Utils.TrdParty.RxQt
#include "RxQt.h"

#include "IRxRecurrentMessageMapper.h"
#include "Message.h"

class RecurrentMessagesCollector {
    using RecurrentMessageMappers_t = std::vector<std::shared_ptr<IRxRecurrentMessageMapper>>;

    using Scheduler_t = rxcpp::observe_on_one_worker;

public:
    RecurrentMessagesCollector(RecurrentMessageMappers_t mappers);

    ~RecurrentMessagesCollector();

    void StartCollectingOn(const Scheduler_t& scheduler);

    QVector<Message> DumpMessages();

private:
    rxcpp::observable<Message> m_obsRecurrentMappers;

    rxcpp::composite_subscription m_subscriptions;

    QHash<QString, Message> m_collectedMessages;

    QMutex m_mtxCollectedMessages;

};

#endif // RECURRENTMESSAGESCOLLECTOR_H
