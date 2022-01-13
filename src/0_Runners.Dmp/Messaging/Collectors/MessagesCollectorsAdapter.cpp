#include "MessagesCollectorsAdapter.h"

// std
#include <chrono>
// qt
#include <QDateTime>
#include <QVector>
#include <QThread>
// oss
#include <plog/Log.h>

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds TIMER_COLLECT_MESSAGES_INTERVAL { 10s };

QString threadId(const QString& name) {
    return name + QString(": %1\n").arg(reinterpret_cast<uintptr_t>(QThread::currentThreadId()));
}

MessagesCollectorsAdapter::MessagesCollectorsAdapter(
    ImmediateMessagesCollector& immediateMessagesCollector,
    RecurrentMessagesCollector& recurrentMessagesCollector,
    MessagesBatchesQueue& queue
)
    : QObject()
    , immediateMessagesCollector { immediateMessagesCollector }
    , recurrentMessagesCollector { recurrentMessagesCollector }
    , queue { queue }
{
    subs = rxcpp::composite_subscription();

    auto recurrentObservable = rxcpp::observable<>
        ::interval(TIMER_COLLECT_MESSAGES_INTERVAL)
        .tap([](auto) { PLOGD << threadId("RECURRENT"); });

    auto immediateObservable =  rxqt
        ::from_signal(&immediateMessagesCollector, &ImmediateMessagesCollector::messageCollected)
        .tap([](auto) { PLOGD << threadId("IMMEDIATE"); });

    recurrentObservable.merge(immediateObservable)
        .observe_on(rxcpp::observe_on_new_thread())
        .tap([](auto) { PLOGD << threadId("CONCAT"); })
        .subscribe(subs, [&](auto) { handleCollectMessages(); });
}

MessagesCollectorsAdapter::~MessagesCollectorsAdapter() {
    subs.unsubscribe();
}

void
MessagesCollectorsAdapter::handleCollectMessages() {
    QMutexLocker lock(&mtxCollectMessagses);

    PLOGD << "HANDLE std " << std::this_thread::get_id();
    PLOGD << threadId("HANDLE qt");

    QVector<Message> messages;
    messages
        << immediateMessagesCollector.getMessages()
        << recurrentMessagesCollector.getMessages();

    if (messages.isEmpty()) {
        return;
    }

    auto messagesCollectedAt { QDateTime::currentDateTimeUtc() };

    auto messagesBatchPlaced = queue.enqueue({ messages, messagesCollectedAt });
    Q_UNUSED(messagesBatchPlaced)
}
