#include "save_messages_batch_command.h"

// qt
#include <QVariant>
// qt sql
#include <QSqlError>
// oss
#include <spdlog/spdlog.h>

#include "utils/spdlog_qt_support.h"

QString DML_INSERT_DEVICE_MESSAGE_BATCH { QStringLiteral(
    "INSERT INTO [messages_batches](\n"
    "    [collected_at]\n"
    ") VALUES (\n"
    "    :collected_at\n"
    ");"
) };

QString DML_INSERT_DEVICE_MESSAGE { QStringLiteral(
    "INSERT INTO [messages](\n"
    "    [messages_batch_id],\n"
    "    [monkier],\n"
    "    [payload],\n"
    "    [produced_at]\n"
    ") VALUES (\n"
    "    :messages_batch_id,\n"
    "    :monkier,\n"
    "    :payload,"
    "    :produced_at\n"
    ");"
) };

void
SaveMessagesBatchCommand::handle(const MessagesBatch& messagesBatch) {
    auto connection = QSqlDatabase::database();

    if (!connection.transaction()) {
        spdlog::error("Could not start transaction: {0}", connection.lastError().text());

        return;
    }

    auto [success, messageBatchId] = insertMessagesBatch(connection, messagesBatch);
    if (!success) {
        connection.rollback();

        return;
    }

    if (!insertMessages(connection, messageBatchId, messagesBatch.messages)) {
        connection.rollback();

        return;
    }

    if (!connection.commit()) {
        spdlog::error("Could not commit transaction: {0}", connection.lastError().text());

        connection.rollback();
    }
}

const QString POS_COLLECTED_AT { QStringLiteral(":collected_at") };

QPair<bool, quint32>
SaveMessagesBatchCommand::insertMessagesBatch(
    const QSqlDatabase& connection,
    const MessagesBatch& messagesBatch
) {
    QSqlQuery query { connection };

    query.prepare(DML_INSERT_DEVICE_MESSAGE_BATCH);
    query.bindValue(POS_COLLECTED_AT, messagesBatch.collectedAt);

    if (!query.exec()) {
        spdlog::error("Could not insert message batch: {0}", query.lastError().text());

        return { false, 0 };
    }

    return { true, query.lastInsertId().toUInt() };
}

const QString POS_MESSAGES_BATCH_ID { QStringLiteral(":messages_batch_id") };
const QString POS_MONKIER { QStringLiteral(":monkier") };
const QString POS_PAYLOAD { QStringLiteral(":payload") };
const QString POS_PRODUCED_AT { QStringLiteral(":produced_at") };

bool
SaveMessagesBatchCommand::insertMessages(
    const QSqlDatabase& connection,
    const qint64 messagesBatchId,
    const QVector<Message>& messages
) {
    QSqlQuery query { connection };

    query.prepare(DML_INSERT_DEVICE_MESSAGE);

    query.bindValue(POS_MESSAGES_BATCH_ID, messagesBatchId);

    for (const auto& message : messages) {
        query.bindValue(POS_MONKIER, message.moniker);
        query.bindValue(POS_PAYLOAD, message.payload);
        query.bindValue(POS_PRODUCED_AT, message.producedAt);

        if (query.exec()) continue;

        spdlog::error("Could not insert message: {0}", query.lastError().text());

        return false;
    }

    return true;
}
