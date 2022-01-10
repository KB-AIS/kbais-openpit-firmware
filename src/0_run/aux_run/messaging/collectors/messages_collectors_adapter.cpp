#include "messages_collectors_adapter.h"

// std
#include <chrono>
// qt
#include <QDateTime>
#include <QVector>

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds TIMER_COLLECT_MESSAGES_INTERVAL { 10s };

MessagesCollectorsAdapter::MessagesCollectorsAdapter(
    ImmediateMessagesCollector& immediateMessagesCollector,
    RecurrentMessagesCollector& recurrentMessagesCollector,
    MessagesBatchesQueue& queue
) : QObject(),
    immediateMessagesCollector { immediateMessagesCollector },
    recurrentMessagesCollector { recurrentMessagesCollector },
    queue { queue } {
    connect(
        &immediateMessagesCollector, &ImmediateMessagesCollector::notifyMessageCollected,

        this, [&] { threadWorker.execInThread([&] { handleCollectMessages(); }); }
    );

    auto loopCollectMessages = threadWorker.startLoopInThread(
        [&] { handleCollectMessages(); }, TIMER_COLLECT_MESSAGES_INTERVAL.count()
    );
    Q_UNUSED(loopCollectMessages)
}

void
MessagesCollectorsAdapter::handleCollectMessages() {
    QMutexLocker lock(&mtxCollectMessagses);

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
