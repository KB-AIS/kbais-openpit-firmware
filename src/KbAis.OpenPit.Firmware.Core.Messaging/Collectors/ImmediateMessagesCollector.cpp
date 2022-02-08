#include "ImmediateMessagesCollector.h"

// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds USR_MESSAGE_DEBOUNCE_PERIOD = 1s;

ImmediateMessagesCollector::ImmediateMessagesCollector(
    ImmediateMessageMappers_t mappers
) {
    m_observableImmediateMappers = mappers.at(0)->getObservable();

    ranges::for_each(
        mappers
    |   ranges::views::drop(1)
    |   ranges::views::transform([](auto x) { return x->getObservable(); })
    ,   [&](const auto& x) {
            m_observableImmediateMappers = m_observableImmediateMappers.merge(x);
        }
    );
}

ImmediateMessagesCollector::~ImmediateMessagesCollector() {
    m_subs.unsubscribe();
}

void
ImmediateMessagesCollector::StartCollectingOn(const Scheduler_t& scheduler) {
    m_subs = rxcpp::composite_subscription();

    const auto messageMapped_f = [&](Message message) {
        //m_mtxCollectedMessages.lock();

        m_collectedMessages.append(message);

        //m_mtxCollectedMessages.unlock();

        m_messagesCollectedSubject.get_subscriber().on_next(true);
    };

    m_observableImmediateMappers
        .observe_on(scheduler)
        .subscribe(m_subs, messageMapped_f);
}

rxcpp::observable<bool>
ImmediateMessagesCollector::GetMessagesCollectedObservable(const Scheduler_t& scheduler) {
    return m_messagesCollectedSubject.get_observable()
        .debounce(USR_MESSAGE_DEBOUNCE_PERIOD, scheduler);
}

QVector<Message>
ImmediateMessagesCollector::DumpMessages() {
    //QMutexLocker lock(&m_mtxCollectedMessages);

    QVector<Message> messages(m_collectedMessages);
    m_collectedMessages.clear();

    return messages;
}
