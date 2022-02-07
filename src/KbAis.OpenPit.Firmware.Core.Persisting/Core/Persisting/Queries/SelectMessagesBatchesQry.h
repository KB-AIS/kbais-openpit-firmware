#ifndef SELECTMESSAGESBATCHESQRY_H
#define SELECTMESSAGESBATCHESQRY_H

// qt sql
#include <QSqlDatabase>

#include "Core/Persisting/Queries/Dtos.h"

class SelectMessagesBatchesQry {

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

#endif // SELECTMESSAGESBATCHESQRY_H
