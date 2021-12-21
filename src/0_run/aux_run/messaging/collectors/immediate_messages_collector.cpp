#include "immediate_messages_collector.h"

// std
#include <chrono>
// qt
#include <QThread>
// plog
#include <plog/Log.h>

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds TIMER_MESSAGE_COLLECTED_DELAY { 1s };

ImmediateMessagesCollector::ImmediateMessagesCollector() : QObject() {
    timerMessageCollected.setSingleShot(true);

    connect(
        &timerMessageCollected, &QTimer::timeout,
        this, &ImmediateMessagesCollector::notifyMessageCollected);
}

QVector<Message>
ImmediateMessagesCollector::getMessages() {
    QMutexLocker lock(&mtxCollectedMessages);

    QVector<Message> messages(collectedMessages);
    collectedMessages.clear();

    return messages;
}

void
ImmediateMessagesCollector::handleMessagePlaced(const Message& message) {
    QMutexLocker lock(&mtxCollectedMessages);

    collectedMessages.append(message);

    // Debounce timer repeate activation.
    if (timerMessageCollected.isActive()) {
        return;
    }

    // Start timer
    timerMessageCollected.start(TIMER_MESSAGE_COLLECTED_DELAY);
}