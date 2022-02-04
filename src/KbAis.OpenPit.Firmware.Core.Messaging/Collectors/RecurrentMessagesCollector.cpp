#include "RecurrentMessagesCollector.h"

// oss
#include "pipes/drop.hpp"
#include "pipes/for_each.hpp"
#include "pipes/operator.hpp"
#include "pipes/push_back.hpp"
#include "pipes/transform.hpp"
#include <range/v3/all.hpp>

RecurrentMessagesCollector::RecurrentMessagesCollector(RecurrentMessageMappers_t mappers) {
    m_observableRecurrentMappers = mappers.at(0)->getObservable();

    mappers
        >>= pipes::drop(1)
        >>= pipes::transform([](auto mapper) {
                return mapper->getObservable();
            })
        >>= pipes::for_each([&](const auto& mapperObservable) {
                m_observableRecurrentMappers = m_observableRecurrentMappers.merge(mapperObservable);
            });
}

RecurrentMessagesCollector::~RecurrentMessagesCollector() {
    m_subscriptions.unsubscribe();
}

void RecurrentMessagesCollector::StartCollectingOn(const Scheduler_t& scheduler) {
    m_subscriptions = rxcpp::composite_subscription();

    m_observableRecurrentMappers
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
