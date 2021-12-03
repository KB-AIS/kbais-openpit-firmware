#include "immediate_messages_collector.h"

// std
#include <chrono>
// Qt
#include <QThread>
// plog
#include <plog/Log.h>

constexpr std::chrono::milliseconds FIRE_DELAY { 1000 };

ImmediateMessagesCollector::ImmediateMessagesCollector(
    QObject *parent
) : QObject(parent) {
    setupFireMessageCollectorTimer();
}

QVector<DeviceMessage>
ImmediateMessagesCollector::popMessages() {
    QMutexLocker lock(&internalStoreMtx);

    QVector<DeviceMessage> msgs(collectedMsgs);
    collectedMsgs.clear();

    return msgs;
}

void
ImmediateMessagesCollector::handleMessageReceived(const DeviceMessage& event) {
    QMutexLocker lock(&internalStoreMtx);

    collectedMsgs.append(event);

    if (fireMessageCollectedTimer.isActive()) {
        return;
    }

    // TODO: move timeout to const
    fireMessageCollectedTimer.start(FIRE_DELAY);
}

void
ImmediateMessagesCollector::setupFireMessageCollectorTimer() {
    fireMessageCollectedTimer.setSingleShot(true);

    connect(
        &fireMessageCollectedTimer, &QTimer::timeout,
        this, [&] {  emit notifyMessageCollected(); }
    );
}
