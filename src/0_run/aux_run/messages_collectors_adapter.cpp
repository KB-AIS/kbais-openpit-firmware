#include "messages_collectors_adapter.h"

#include "database_factory.h"
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

constexpr std::chrono::milliseconds COLLECT_MSGS_TIMEOUT { 10s };

MessagesCollectorsAdapter::MessagesCollectorsAdapter(
    RecurrentMessagesCollector& recurrentCollector,
    ImmediateMessagesCollector& immediateCollector,
    BlockingReaderWriterQueue<DeviceMessage>& msgsQueue,
    QObject* parent
) : QObject(parent),
    recurrentCollector { recurrentCollector },
    immediateCollector { immediateCollector },
    _msgsQueue { msgsQueue } {
    connect(
        &immediateCollector, &ImmediateMessagesCollector::notifyMessageCollected,

        this, [&] { collectMessages(); }
    );

    // TODO: Setup iteration time from configuration
    _trdWorker.startLoopInThread([&] { collectMessages(); }, COLLECT_MSGS_TIMEOUT.count());
}

void
MessagesCollectorsAdapter::collectMessages() {
    QMutexLocker lock(&collectMsgsMtx);

    QVector<DeviceMessage> messages;
    messages.append(immediateCollector.popMessages());
    messages.append(recurrentCollector.popMessages());

    if (messages.isEmpty()) {
        PLOGD << "No messags has been collected to send";
        return;
    }

    _msgsQueue.emplace(messages.first());
}
