#include "get_messages_batches_query.h"

// qt
#include <QMap>
#include <QVariant>
// qt sql
#include <QSqlError>
#include <QSqlQuery>
// oss
#include <spdlog/spdlog.h>

#include "utils/spdlog_qt_support.h"

static QString QML_SELECT_DEVICE_MESSAGE_BATCHES { QStringLiteral(
    "SELECT\n"
    "   [mb].[id]           AS messagesBatch_id,\n"
    "   [mb].[collected_at] AS messagesBatch_collectedAt,\n"
    "   [m].[id]            AS message_id,\n"
    "   [m].[monkier]       AS message_monkier,\n"
    "   [m].[payload]       AS message_payload,\n"
    "   [m].[produced_at]   AS message_producedAt\n"
    "FROM\n"
    "   [messages_batches] AS [mb]\n"
    "LEFT JOIN\n"
    "   [messages] AS [m] ON [mb].[id] = [m].[messages_batch_id]\n"
    "WHERE\n"
    "   [mb].[id] > :last_sent_messages_batch_id\n"
    "ORDER BY\n"
    "   [mb].[id] ASC\n"
    "LIMIT\n"
    "   :messages_batches_count;"
) };

const QString QML_SELECT_LAST_SENT_MESSAGES_BATCH_ID { QStringLiteral(
    "SELECT\n"
    "   [s].[last_sent_messages_batch_id] AS sender_lastSentMessagesBatchId\n"
    "FROM\n"
    "   [senders] AS [s]\n"
    "WHERE\n"
    "   [s].[host] = '10.214.1.208' AND [s].[port] = 9900;"
) };

QList<MessagesBatchDto>
GetMessagesBatchesQuery::handle(qint32 messagesBatchesCount) const {
    auto c = QSqlDatabase::database();

    quint64 lastSentMessagesBatchId;
    if (!getLastSentMessagesBatchId(c, lastSentMessagesBatchId)) {
        return { };
    }

    QList<MessagesBatchDto> messagesBatches;
    if (!getMessagesBatches(c, lastSentMessagesBatchId, messagesBatchesCount, messagesBatches)) {
        return { };
    }

    return messagesBatches;
}

bool
GetMessagesBatchesQuery::getLastSentMessagesBatchId(
    const QSqlDatabase& connection,
    quint64& messageBatchIdOut
) const {
    Q_UNUSED(messageBatchIdOut)

    QSqlQuery query { connection };

    query.prepare(QML_SELECT_LAST_SENT_MESSAGES_BATCH_ID);

    if (!query.exec()) {
        spdlog::error("Could not select last sent messages batch id: {0}",
                      query.lastError().text());

        return false;
    }
    query.first();

    bool queryFieldConverted;
    auto lastSentMessagesBatchId =
        query.value("sender_lastSentMessagesBatchId").toULongLong(&queryFieldConverted);

    if (!queryFieldConverted) {
        spdlog::warn("Could not convert last sent messages batch id to uint64");

        return false;
    }

    messageBatchIdOut = lastSentMessagesBatchId;

    return true;
}

bool
GetMessagesBatchesQuery::getMessagesBatches(
    const QSqlDatabase &connection,
    quint64 lastSentMessagesBatchId,
    qint32 messagesBatchesCount,
    QList<MessagesBatchDto>& messagesBatchesOut
) const {
    QSqlQuery query { connection };

    query.prepare(QML_SELECT_DEVICE_MESSAGE_BATCHES);
    query.bindValue(":last_sent_messages_batch_id", lastSentMessagesBatchId);
    query.bindValue(":messages_batches_count", messagesBatchesCount);

    if (!query.exec()) {
        spdlog::error("Could not select messages batches: {0}", query.lastError().text());

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
