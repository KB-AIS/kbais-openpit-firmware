#include "messages_collectors_adapter.h"

#include "database_configuration.h"
// std
#include <chrono>
// Qt
#include <QDateTime>
#include <QList>
#include <QThread>
#include <QVector>
// plog
#include <plog/Log.h>

using namespace std::chrono_literals;

using MessagesQueue = moodycamel::BlockingReaderWriterQueue<DeviceMessage>;

constexpr std::chrono::milliseconds COLLECT_MSGS_TIMEOUT { 10s };

MessagesCollectorsAdapter::MessagesCollectorsAdapter(
    RecurrentMessagesCollector& recurrentCollector,
    ImmediateMessagesCollector& immediateCollector,
    MessagesQueue& messagesQueue,
    QObject* parent
) : QObject(parent),
    recurrentCollector { recurrentCollector },
    immediateCollector { immediateCollector },
    messagesQueue { messagesQueue } {
    connect(
        &immediateCollector, &ImmediateMessagesCollector::notifyMessageCollected,

        this, [&] { collectMessages(); }
    );

    // TODO: Setup iteration time from configuration
    threadWorker.startLoopInThread([&] { collectMessages(); }, COLLECT_MSGS_TIMEOUT.count());
}

void
MessagesCollectorsAdapter::collectMessages() {
    QMutexLocker lock(&collectMsgsMtx);

    QVector<DeviceMessage> messages;
    messages.append(immediateCollector.popMessages());
    messages.append(recurrentCollector.popMessages());

    if (messages.isEmpty()) {
        return;
    }

    messagesQueue.emplace(messages.first());
}
