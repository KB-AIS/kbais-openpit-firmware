#include "messages_batches_send_queue.h"

// std
#include <algorithm>

using namespace std::chrono;

MessagesBatchesSendQueue::MessagesBatchesSendQueue(qint32 capacity, QObject *parent) :
    capacity { capacity },
    internalQueue { QVector<MessagesBatchDto>() },
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
    auto enequeCount = remaningCapacity();

    auto messagesBatchesCount = messagesBatches.count();

    if (messagesBatchesCount < enequeCount) {
        enequeCount = messagesBatches.count();
    }

    if (messagesBatchesCount > enequeCount) {
        enequeCount -= messagesBatchesCount - enequeCount;
    }

    std::copy_n(messagesBatches.begin(), enequeCount, std::back_inserter(internalQueue));
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
