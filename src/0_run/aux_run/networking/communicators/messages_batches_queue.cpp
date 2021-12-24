#include "messages_batches_queue.h"

// std
#include <algorithm>

using namespace std::chrono;

MessagesBatchesQueue::MessagesBatchesQueue(qint32 capacity, QObject *parent) :
    capacity { capacity },
    internalQueue { QVector<MessagesBatchDto>(capacity) },
    QObject(parent) { }

void
MessagesBatchesQueue::requestPeek(quint32 size) const {
    if (size > capacity) {
        size -= size - capacity;
    }

    const auto peekedMssagesBatches = QVector<MessagesBatchDto>(size);
    std::copy_n(internalQueue.begin(), size, peekedMssagesBatches);

    emit notifyPeeked(peekedMssagesBatches);
}

void
MessagesBatchesQueue::enqueue(const QVector<MessagesBatchDto>& messagesBatches) {
    auto size = remaningCapacity();
    if (messagesBatches.size() > size) {
        size = messagesBatches.size();
    }

    std::copy_n(messagesBatches.begin(), size, internalQueue);
}

void
MessagesBatchesQueue::dequeueVoid(quint32 size) {
    if (size > capacity) {
        size -= size - capacity;
    }

    internalQueue.erase(internalQueue.begin(), internalQueue.begin() + size);
}

qint32
MessagesBatchesQueue::remaningCapacity() const noexcept {
    return capacity - internalQueue.size();
}
