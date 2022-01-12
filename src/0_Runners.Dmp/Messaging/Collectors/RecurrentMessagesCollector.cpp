#include "RecurrentMessagesCollector.h"

// std
#include <algorithm>

RecurrentMessagesCollector::RecurrentMessagesCollector()
    : QObject() {}

QVector<Message>
RecurrentMessagesCollector::getMessages() {
    QMutexLocker lock(&mtxCollectedMessages);

    QVector<Message> messages { collectedMessages.size() };
    std::copy(collectedMessages.begin(), collectedMessages.end(), std::back_inserter(messages));

    return messages;
}

void
RecurrentMessagesCollector::storeMessage(const Message& message) {
    QMutexLocker locker(&mtxCollectedMessages);

    collectedMessages.insert(message.moniker, message);
}
