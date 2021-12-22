#include "save_messages_batch_command.h"

// qt
#include <QVariant>
// qt sql
#include <QSqlError>
// oss
#include <plog/Log.h>

QString DML_INSERT_DEVICE_MESSAGE_BATCH { QStringLiteral(
    "INSERT INTO [device_message_batches](\n"
    "    [id]\n"
    ") VALUES (\n"
    "    NULL\n"
    ");"
) };

QString DML_INSERT_DEVICE_MESSAGE { QStringLiteral(
    "INSERT INTO [device_messages](\n"
    "    batch_id,\n"
    "    monkier,\n"
    "    payload\n"
    ") VALUES (\n"
    "    :batch_id,\n"
    "    :monkier,\n"
    "    :payload\n"
    ");"
) };

void
SaveMessagesBatchCommand::handle(const MessagesBatch& messsagesBatch) {
    auto connection = QSqlDatabase::database();

    if (!connection.transaction()) {
        PLOGE << "Could not start transaction: \n" << connection.lastError().text();;

        return;
    }

    auto [success, messageBatchId] = insertMessagesBatch(connection);
    if (!success) {
        PLOGE << "Could not insert message batch: \n" << connection.lastError().text();

        connection.rollback();

        return;
    }

    if (!insertMessages(connection, messageBatchId, messsagesBatch.messages)) {
        PLOGD << "Could not insert message: \n" << connection.lastError().text();

        connection.rollback();

        return;
    }

    if (!connection.commit()) {
        PLOGE << "Could not commit transaction: \n" << connection.lastError().text();

        connection.rollback();
    }
}

QPair<bool, quint32>
SaveMessagesBatchCommand::insertMessagesBatch(const QSqlDatabase &connection) {
    QSqlQuery query { connection };

    if (!query.exec(DML_INSERT_DEVICE_MESSAGE_BATCH)) {
        return { false, 0 };
    }

    return { true, query.lastInsertId().toUInt() };
}

bool
SaveMessagesBatchCommand::insertMessages(
    const QSqlDatabase& connection,
    const qint64 messagesBatchId,
    const QVector<Message>& messages
) {
    const QString POS_BATCH_ID { QStringLiteral(":batch_id") };
    const QString POS_MONKIER  { QStringLiteral(":monkier") };
    const QString POS_PAYLOAD  { QStringLiteral(":payload") };

    QSqlQuery query { connection };

    query.prepare(DML_INSERT_DEVICE_MESSAGE);

    query.bindValue(POS_BATCH_ID, messagesBatchId);

    for (const auto& message : messages) {
        query.bindValue(POS_MONKIER, message.moniker);
        query.bindValue(POS_PAYLOAD, message.payload);

        if (!query.exec()) {
            return false;
        }
    }
}
