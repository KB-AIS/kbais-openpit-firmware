#include "recurrent_messages_collector.h"

// qt
#include <QtAlgorithms>

RecurrentMessagesCollector::RecurrentMessagesCollector() : QObject() {}

QVector<Message>
RecurrentMessagesCollector::getMessages() {
    QMutexLocker lock(&mtxCollectedMessages);

    QVector<Message> messages { collectedMessages.size() };
    qCopy(collectedMessages.begin(), collectedMessages.end(), messages .begin());

    return messages ;
}

void
RecurrentMessagesCollector::handleMessagePlaced(const Message& message) {
    QMutexLocker locker(&mtxCollectedMessages);

    collectedMessages.insert(message.moniker, message);
}
