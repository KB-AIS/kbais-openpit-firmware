#include "SelectMessagesBatchesQry.h"

// qt
#include <QMap>
#include <QVariant>
// qt sql
#include <QSqlError>
#include <QSqlQuery>
// oss
#include <plog/Log.h>

static const QString QML_SELECT_MESSAGES { QStringLiteral(
    R"(
        WITH
            [messages_batches_sub] AS (
                SELECT
                    [mb].id,
                    [mb].collected_at
                FROM
                    [messages_batches] AS [mb]
                WHERE
                    [mb].[id] > :last_sent_messages_batch_id
                LIMIT
                    :messages_batches_count
            )
        SELECT
            [mb].[id]           AS messagesBatch_id,
            [mb].[collected_at] AS messagesBatch_collectedAt,
            [m].[id]            AS message_id,
            [m].[monkier]       AS message_monkier,
            [m].[payload]       AS message_payload,
            [m].[produced_at]   AS message_producedAt
        FROM
            [messages_batches_sub] AS [mb]
        LEFT JOIN
            [messages] AS [m] ON [mb].[id] = [m].[messages_batch_id]
        ORDER BY
            [mb].[id] ASC;
    )"
) };

static const QString QML_SELECT_LAST_SENT_MESSAGES_BATCH_ID { QStringLiteral(
    R"(
        SELECT
            [s].[last_sent_messages_batch_id] AS sender_lastSentMessagesBatchId
        FROM
            [senders] AS [s]
        WHERE
            [s].[host] = '10.214.1.208' AND [s].[port] = 9900;
    )"
) };

QVector<MessagesBatchDto>
MessageBatchGetAllQry::handle(qint32 messagesBatchesCount) const {
    auto c = QSqlDatabase::database();

    quint64 lastSentMessagesBatchId;
    if (!getLastSentMessagesBatchId(c, lastSentMessagesBatchId)) {
        return { };
    }

    QList<MessagesBatchDto> messagesBatches;
    if (!getMessagesBatches(c, lastSentMessagesBatchId, messagesBatchesCount, messagesBatches)) {
        return { };
    }

    return messagesBatches.toVector();
}

bool
MessageBatchGetAllQry::getLastSentMessagesBatchId(
    const QSqlDatabase& connection,
    quint64& messageBatchIdOut
) const {
    Q_UNUSED(messageBatchIdOut)

    QSqlQuery query { connection };

    query.prepare(QML_SELECT_LAST_SENT_MESSAGES_BATCH_ID);

    if (!query.exec()) {
        PLOGE << "Could not select last sent messages batch id: " << query.lastError().text();

        return false;
    }
    query.first();

    bool queryFieldConverted;
    auto lastSentMessagesBatchId =
        query.value("sender_lastSentMessagesBatchId").toULongLong(&queryFieldConverted);

    if (!queryFieldConverted) {
        PLOGW << "Could not convert last sent messages batch id to uint64";

        return false;
    }

    messageBatchIdOut = lastSentMessagesBatchId;

    return true;
}

bool
MessageBatchGetAllQry::getMessagesBatches(
    const QSqlDatabase &connection,
    quint64 lastSentMessagesBatchId,
    qint32 messagesBatchesCount,
    QList<MessagesBatchDto>& messagesBatchesOut
) const {
    QSqlQuery query { connection };

    query.prepare(QML_SELECT_MESSAGES);
    query.bindValue(":last_sent_messages_batch_id", lastSentMessagesBatchId);
    query.bindValue(":messages_batches_count", messagesBatchesCount);

    if (!query.exec()) {
        PLOGE << "Could not select messages batches: " << query.lastError().text();

        return false;
    }

    QMap<quint64, MessagesBatchDto> messagesBatches;
    while (query.next()) {
        auto messagesBatchId = query.value("messagesBatch_id").toULongLong();

        if (!messagesBatches.contains(messagesBatchId)) {
            messagesBatches[messagesBatchId] = {
                messagesBatchId,
                { },
                query.value("messagesBatch_collectedAt").toDateTime(),
            };
        }

        messagesBatches[messagesBatchId].messages.push_back({
            query.value("message_id").toULongLong(),
            query.value("message_monkier").toString(),
            query.value("message_payload").toByteArray(),
            query.value("message_producedAt").toDateTime(),
        });
    }

    messagesBatchesOut << messagesBatches.values();

    return true;
}
