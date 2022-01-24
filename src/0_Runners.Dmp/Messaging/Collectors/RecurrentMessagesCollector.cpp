#include "RecurrentMessagesCollector.h"

// std
#include <algorithm>

RecurrentMessagesCollector::RecurrentMessagesCollector(
    const DmpRecurrentMessagesMapService& messagesMapSerivce
)
    : QObject()
{
    messagesMapSerivce.getMessageObservable().subscribe([&](auto x) {
        QMutexLocker locker(&mMtxCollectedMessages);

        mCollectedMessages.insert(x.moniker, x);
    });
}

QVector<Message>
RecurrentMessagesCollector::getMessages() {
    QMutexLocker lock(&mMtxCollectedMessages);

    QVector<Message> messages;
    std::copy(
        mCollectedMessages.constBegin()
    ,   mCollectedMessages.constEnd()
    ,   std::back_inserter(messages)
    );

    return messages;
}
