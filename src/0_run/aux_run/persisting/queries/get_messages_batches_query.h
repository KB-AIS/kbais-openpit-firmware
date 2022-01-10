#ifndef GET_MESSAGES_BATCHES_QUERY_H
#define GET_MESSAGES_BATCHES_QUERY_H

// qt sql
#include <QSqlDatabase>

#include "persisting/queries/dtos.h"

class GetMessagesBatchesQuery {

public:
    QVector<MessagesBatchDto> handle(qint32 count) const;

private:
    bool getLastSentMessagesBatchId(
        const QSqlDatabase& connection,
        quint64& messageBatchIdOut
    ) const;

    bool getMessagesBatches(
        const QSqlDatabase& connection,
        quint64 lastSentMessagesBatchId,
        qint32 messagesBatchesCount,
        QList<MessagesBatchDto>& messagesBatchesOut
    ) const;

};

#endif // GET_MESSAGES_BATCHES_QUERY_H
