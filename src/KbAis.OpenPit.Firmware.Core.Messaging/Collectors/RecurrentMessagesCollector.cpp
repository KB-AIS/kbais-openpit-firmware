#include "RecurrentMessagesCollector.h"

// oss
#include "pipes/drop.hpp"
#include "pipes/for_each.hpp"
#include "pipes/operator.hpp"
#include "pipes/push_back.hpp"
#include "pipes/transform.hpp"

RecurrentMessagesCollector::RecurrentMessagesCollector(RecurrentMessageMappers_t mappers) {
    mObservable = mappers.at(0)->getObservable();

    mappers
        >>= pipes::drop(1)
        >>= pipes::transform([](auto mapper) {
                return mapper->getObservable();
            })
        >>= pipes::for_each([&](const auto& mapperObservable) {
                mObservable = mObservable.merge(mapperObservable);
            });
}

RecurrentMessagesCollector::~RecurrentMessagesCollector() {
    mSubs.unsubscribe();
}

void RecurrentMessagesCollector::startCollectingOn(const rxqt::run_loop& loop) {
    mSubs = rxcpp::composite_subscription();

    mObservable
        .observe_on(loop.observe_on_run_loop())
        .subscribe(
            mSubs
        ,   [&](Message message) {
                QMutexLocker locker(&mMtxMessages);
                mMessages.insert(message.moniker, message);
            }
        );
}

QVector<Message>
RecurrentMessagesCollector::dumpMessages() {
    QMutexLocker lock(&mMtxMessages);

    QVector<Message> messages;
    std::copy(std::begin(mMessages), std::end(mMessages), pipes::push_back(messages));

    return messages;
}
