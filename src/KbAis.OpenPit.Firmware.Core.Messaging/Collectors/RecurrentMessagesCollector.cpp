#include "RecurrentMessagesCollector.h"

// oss
#include <range/v3/all.hpp>

RecurrentMessagesCollector::RecurrentMessagesCollector(RecurrentMessageMappers_t mappers) {
    m_obsRecurrentMappers = mappers.at(0)->getObservable();

    ranges::for_each(
        mappers
    |   ranges::views::drop(1)
    |   ranges::views::transform([](auto x) { return x->getObservable(); })
    ,   [&o = m_obsRecurrentMappers](const auto& x) { o = o.merge(x); }
    );
}

RecurrentMessagesCollector::~RecurrentMessagesCollector() {
    m_subscriptions.unsubscribe();
}

void RecurrentMessagesCollector::StartCollectingOn(const Scheduler_t& scheduler) {
    m_subscriptions = rxcpp::composite_subscription();

    m_obsRecurrentMappers
        .observe_on(scheduler)
        .subscribe(
            m_subscriptions
        ,   [&](Message message) {
                //QMutexLocker locker(&m_mtxCollectedMessages);
                m_collectedMessages.insert(message.moniker, message);
            }
        );
}

QVector<Message>
RecurrentMessagesCollector::DumpMessages() {
    //QMutexLocker lock(&m_mtxCollectedMessages);

    QVector<Message> messages { m_collectedMessages.values().toVector() };

    return messages;
}
