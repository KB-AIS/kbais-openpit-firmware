#include "messages_batches_queue.h"

// std
#include <algorithm>

using namespace std::chrono;

MessagesBatchesSendQueue::MessagesBatchesSendQueue(qint32 capacity, QObject *parent) :
    capacity { capacity },
    internalQueue { QVector<MessagesBatchDto>(capacity) },
    QObject(parent) { }

void
MessagesBatchesSendQueue::requestPeek(quint32 size) const {
    if (size > capacity) {
        size -= size - capacity;
    }

    auto peekedMssagesBatches = QVector<MessagesBatchDto>(size);
    std::copy_n(internalQueue.begin(), size, std::back_inserter(peekedMssagesBatches));

    emit notifyPeeked(peekedMssagesBatches);
}

void
MessagesBatchesSendQueue::enqueue(const QVector<MessagesBatchDto>& messagesBatches) {
    auto size = remaningCapacity();
    if (messagesBatches.size() > size) {
        size = messagesBatches.size();
    }

    std::copy_n(messagesBatches.begin(), size, std::back_inserter(internalQueue));
}

void
MessagesBatchesSendQueue::dequeueVoid(quint32 size) {
    if (size > capacity) {
        size -= size - capacity;
    }

    internalQueue.erase(internalQueue.begin(), internalQueue.begin() + size);
}

qint32
MessagesBatchesSendQueue::remaningCapacity() const noexcept {
    return capacity - internalQueue.size();
}
