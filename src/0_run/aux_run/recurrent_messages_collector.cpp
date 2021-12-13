#include "recurrent_messages_collector.h"

// Qt
#include <QtAlgorithms>

RecurrentMessagesCollector::RecurrentMessagesCollector(
    QObject* parent
) : QObject(parent) {}

QVector<DeviceMessage>
RecurrentMessagesCollector::popMessages() {
    QMutexLocker locker(&internalStoreMtx);

    QVector<DeviceMessage> msgs { collectedMsgs.size() };
    qCopy(collectedMsgs.begin(), collectedMsgs.end(), msgs.begin());

    return msgs;
}

void
RecurrentMessagesCollector::handleMessageReceived(const DeviceMessage& event) {
    QMutexLocker locker(&internalStoreMtx);

    collectedMsgs.insert(event.moniker, event);
}
