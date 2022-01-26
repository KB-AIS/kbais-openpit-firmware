#include "ImmediateMessagesCollector.h"

// std
#include <chrono>
// qt
#include <QThread>

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds TIMER_MESSAGE_COLLECTED_DELAY { 1s };

ImmediateMessagesCollector::ImmediateMessagesCollector() : QObject() {
    timerMessageCollected.setSingleShot(true);

    connect(
        &timerMessageCollected, &QTimer::timeout,
        this, &ImmediateMessagesCollector::messageCollected);
}

QVector<Message>
ImmediateMessagesCollector::getMessages() {
    QMutexLocker lock(&mtxCollectedMessages);

    QVector<Message> messages(collectedMessages);
    collectedMessages.clear();

    return messages;
}

void
ImmediateMessagesCollector::placeMessage(const Message& message) {
    QMutexLocker lock(&mtxCollectedMessages);

    collectedMessages.append(message);

    // Debounce timer repeate activation.
    if (timerMessageCollected.isActive()) {
        return;
    }

    // Start timer
    timerMessageCollected.start(TIMER_MESSAGE_COLLECTED_DELAY);
}
