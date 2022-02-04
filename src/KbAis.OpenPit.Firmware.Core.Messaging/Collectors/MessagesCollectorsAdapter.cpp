#include "MessagesCollectorsAdapter.h"

// std
#include <chrono>
// qt
#include <QDateTime>
#include <QVector>
#include <QThread>
// oss
#include <fmt/core.h>
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
MessagesCollectorsAdapter::StartCollectingOn(const rxcpp::observe_on_one_worker& scheduler) {
    mSubs = rxcpp::composite_subscription();

    auto immediateObservable =
        mImmediateMessagesCollector.GetMessagesCollectedObservable(scheduler);

    auto recurrentObservable = rxcpp::observable<>
        ::interval(COLLECT_MESSAGES_PERIOD, scheduler)
        .map([](auto) { return true; });

    recurrentObservable.merge(immediateObservable)
        //.subscribe_on(rxcpp::observe_on_new_thread())
        .subscribe(mSubs, [&](auto) { handleCollectMessages(); });

    mImmediateMessagesCollector.StartCollectingOn(scheduler);

    mRecurrentMessagesCollector.StartCollectingOn(scheduler);
}

void
MessagesCollectorsAdapter::handleCollectMessages() {
    PLOGD << "Messages collector started to dump messages";

    QVector<Message> messages;
    messages
        << mImmediateMessagesCollector.DumpMessages()
        << mRecurrentMessagesCollector.DumpMessages();

    if (messages.isEmpty()) {
        PLOGD << "Messages collector has no messages to enqueue";

        return;
    }

    PLOGD << fmt::format("Messages collector dumped {} messages", messages.size());

    auto messagesCollectedAt { QDateTime::currentDateTimeUtc() };

    auto messagesBatchPlaced = mQueue.enqueue({ messages, messagesCollectedAt });

    if (!messagesBatchPlaced) {
        PLOGW << "Messages collector could not enqueue messsages";

        return;
    }

    PLOGD << "Messages collector enqueued messages";
}
