#include "MessagesCollectorsAdapter.h"

// std
#include <chrono>
// oss
#include <fmt/format.h>
#include <plog/Log.h>

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds COLLECT_MESSAGES_PERIOD { 10s };

MessagesCollectorsAdapter::MessagesCollectorsAdapter(
    ImmediateMessagesCollector& immediateMessagesCollector
,   RecurrentMessagesCollector& recurrentMessagesCollector
,   MessagesBatchesCachingQueue_t& queue
)
    :   QObject()
    ,   mImmediateMessagesCollector { immediateMessagesCollector }
    ,   mRecurrentMessagesCollector { recurrentMessagesCollector }
    ,   mQueue { queue }
{

}

MessagesCollectorsAdapter::~MessagesCollectorsAdapter() {
    mSubs.unsubscribe();
}

void
MessagesCollectorsAdapter::StartCollectingOn(const rxcpp::observe_on_one_worker& coordinator) {
    mSubs = rxcpp::composite_subscription();

    auto immediateObservable =
        mImmediateMessagesCollector.GetMessagesCollectedObservable(coordinator);

    auto recurrentObservable = rxcpp::observable<>
        ::interval(COLLECT_MESSAGES_PERIOD, coordinator)
        .map([](auto) { return true; });

    recurrentObservable.merge(immediateObservable)
        .subscribe(mSubs, [&](auto) { handleCollectMessages(); });

    mImmediateMessagesCollector.StartCollectingOn(coordinator);

    mRecurrentMessagesCollector.StartCollectingOn(coordinator);
}

void
MessagesCollectorsAdapter::handleCollectMessages() {
    PLOGV << "startin to dump messages";

    QVector<Message> messages;
    messages << mImmediateMessagesCollector.DumpMessages()
             << mRecurrentMessagesCollector.DumpMessages();

    if (messages.isEmpty()) {
        PLOGV << "has no messages to enqueue";

        return;
    }

    PLOGV << "dumped " << messages.size() << "messages";

    auto messagesCollectedAt { QDateTime::currentDateTimeUtc() };

    auto messagesBatchPlaced = mQueue.enqueue({ messages, messagesCollectedAt });

    if (!messagesBatchPlaced) {
        PLOGW << "could not enqueue messsages";

        return;
    }

    PLOGV << "enqueued messages";
}
