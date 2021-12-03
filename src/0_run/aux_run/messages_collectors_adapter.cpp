#include "messages_collectors_adapter.h"

// Qt
#include <QThread>
#include <QDateTime>
#include <QList>
#include <QVector>
// plog
#include <plog/Log.h>

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
    worker.startLoopInThread([&] { collectMessages(); }, 10 * 1000);
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
