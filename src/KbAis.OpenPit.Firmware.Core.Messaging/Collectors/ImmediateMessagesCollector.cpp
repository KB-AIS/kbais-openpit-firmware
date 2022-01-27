#include "ImmediateMessagesCollector.h"

// oss
#include <pipes/drop.hpp>
#include <pipes/for_each.hpp>
#include <pipes/operator.hpp>
#include <pipes/push_back.hpp>
#include <pipes/transform.hpp>
#include <plog/Log.h>

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds USR_MESSAGE_DEBOUNCE_PERIOD = 1s;

ImmediateMessagesCollector::ImmediateMessagesCollector(ImmediateMessageMappers_t mappers) {
    mSubject = rxcpp::rxsub::subject<long>();

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

ImmediateMessagesCollector::~ImmediateMessagesCollector() {
    mSubscriptions.unsubscribe();
}

void
ImmediateMessagesCollector::startCollectingOn(const rxqt::run_loop& loop) {
    mSubscriptions = rxcpp::composite_subscription();

    mObservable
        .observe_on(loop.observe_on_run_loop())
        .subscribe(
            mSubscriptions
        ,   [&](Message message) {
                mMtxMessages.lock();
                mMessages.append(message);
                mMtxMessages.unlock();

                mSubject.get_subscriber().on_next(0);
            }
   );
}

rxcpp::observable<long>
ImmediateMessagesCollector::getCollectedObservable() {
    return mSubject.get_observable()
        .debounce(USR_MESSAGE_DEBOUNCE_PERIOD);
}

QVector<Message>
ImmediateMessagesCollector::dumpMessages() {
    QMutexLocker lock(&mMtxMessages);

    QVector<Message> messages(mMessages);
    mMessages.clear();

    return messages;
}
