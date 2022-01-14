#ifndef MESSAGESBATCHESSENDQUEUE_H
#define MESSAGESBATCHESSENDQUEUE_H

// qt
#include <QObject>
#include <QQueue>

#include "Persisting/Queries/Dtos.h"

class MessagesBatchesSendQueue : public QObject {
    Q_OBJECT

public:
    explicit MessagesBatchesSendQueue(qint32 capacity, QObject *parent = nullptr);

    void requestPeek(quint32 size) const;

    void enqueue(const QVector<MessagesBatchDto>& messagesBatches);

    void dequeueVoid(quint32 size);

    qint32 remaningCapacity() const noexcept;

    Q_SIGNAL void notifyPeeked(const QVector<MessagesBatchDto>& messagesBatches) const;

private:
    qint32 capacity;

    QVector<MessagesBatchDto> internalQueue;

};

#endif // MESSAGESBATCHESSENDQUEUE_H
