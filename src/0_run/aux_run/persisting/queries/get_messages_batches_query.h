#ifndef GET_MESSAGES_BATCHES_QUERY_H
#define GET_MESSAGES_BATCHES_QUERY_H

// qt
#include <QDateTime>
#include <QVector>
// qt sql
#include <QSqlDatabase>

struct MessageDto {
    quint64 id;

    QString moniker;

    QByteArray payload;

    QDateTime producedAt;
};

struct MessagesBatchDto {
    quint64 id;

    QVector<MessageDto> messages;

    QDateTime collectedAt;
};

class GetMessagesBatchesQuery {

public:
    QList<MessagesBatchDto> handle(qint32 count) const;

private:
    bool getLastSentMessagesBatchId(
        const QSqlDatabase& connection,
        quint64* messageBatchIdOut
    ) const;

    bool getMessagesBatches(
        const QSqlDatabase& connection,
        quint64 lastSentMessagesBatchId,
        qint32 messagesBatchesCount,
        QList<MessagesBatchDto>& messagesBatchesOut
    ) const;

};

#endif // GET_MESSAGES_BATCHES_QUERY_H
