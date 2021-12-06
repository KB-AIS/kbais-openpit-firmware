#include "messages_collectors_adapter.h"

// std
#include <chrono>
// Qt
#include <QThread>
#include <QDateTime>
#include <QList>
#include <QVector>
// plog
#include <plog/Log.h>

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds COLLECT_MSGS_TIMEOUT { 10s };

MessagesCollectorsAdapter::MessagesCollectorsAdapter(
    RecurrentMessagesCollector& recurrentCollector,
    ImmediateMessagesCollector& immediateCollector,
    QObject* parent
) : QObject(parent),
    recurrentCollector(recurrentCollector),
    immediateCollector(immediateCollector) {

    connect(
        &immediateCollector, &ImmediateMessagesCollector::notifyMessageCollected,

        this, [&] { collectMessages(); }
    );

    // TODO: Setup iteration time from configuration
    threadWorker.startLoopInThread([&] { collectMessages(); }, COLLECT_MSGS_TIMEOUT.count());
}

void
MessagesCollectorsAdapter::collectMessages() {
    QMutexLocker lock(&collectMessagesMtx);

    PLOGD << "Collected at " << QDateTime::currentDateTime().toString();

    QVector<DeviceMessage> foo;
    foo.append(immediateCollector.popMessages());
    foo.append(recurrentCollector.popMessages());

    for (auto& message : foo) {
        PLOGD << message.moniker << " -- " << message.payload;
    }
}
