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

constexpr std::chrono::milliseconds TIMER_COLLECT_MESSAGES_INTERVAL { 10s };

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
,   MessagesBatchesQueue& queue
)
    :   QObject()
    ,   immediateMessagesCollector { immediateMessagesCollector }
    ,   recurrentMessagesCollector { recurrentMessagesCollector }
    ,   queue { queue }
{
    subs = rxcpp::composite_subscription();

    auto recurrentObservable = rxcpp::observable<>
        ::interval(TIMER_COLLECT_MESSAGES_INTERVAL);

    auto immediateObservable = rxqt
        ::from_signal(&immediateMessagesCollector, &ImmediateMessagesCollector::messageCollected);

    recurrentObservable.merge(immediateObservable)
        .subscribe_on(rxcpp::observe_on_new_thread())
        .subscribe(subs, [&](auto) { handleCollectMessages(); });
}

MessagesCollectorsAdapter::~MessagesCollectorsAdapter() {
    subs.unsubscribe();
}

void
MessagesCollectorsAdapter::handleCollectMessages() {
    PLOGD << threadId("HANDLE");

    QVector<Message> messages;
    messages
        << immediateMessagesCollector.getMessages()
        << recurrentMessagesCollector.getMessages();

    if (messages.isEmpty()) return;

    auto messagesCollectedAt { QDateTime::currentDateTimeUtc() };

    auto messagesBatchPlaced = queue.enqueue({ messages, messagesCollectedAt });
    Q_UNUSED(messagesBatchPlaced)
}
