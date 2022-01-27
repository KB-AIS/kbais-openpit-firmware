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

std::string threadId(const QString& name) {
    return fmt::format(
        "{}: {}"
    ,   name.toStdString()
    ,   reinterpret_cast<uintptr_t>(QThread::currentThreadId())
    );
}

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
    mSubs = rxcpp::composite_subscription();

    auto immediateObservable = immediateMessagesCollector.getCollectedObservable();

    auto recurrentObservable = rxcpp::observable<>
        ::interval(COLLECT_MESSAGES_PERIOD);

    recurrentObservable.merge(immediateObservable)
        .subscribe_on(rxcpp::observe_on_new_thread())
        .subscribe(mSubs, [&](auto) { handleCollectMessages(); });
}

MessagesCollectorsAdapter::~MessagesCollectorsAdapter() {
    mSubs.unsubscribe();
}

void
MessagesCollectorsAdapter::handleCollectMessages() {
    PLOGD << "Messages collector started to dump messages";

    QVector<Message> messages;
    messages
        << mImmediateMessagesCollector.dumpMessages()
        << mRecurrentMessagesCollector.dumpMessages();

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
