#include "save_device_messages_command.h"

// Qt
#include <QVariant>
// Qt SQL
#include <QSqlError>
// plog
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
SaveDeviceMessagesCommand::execute(const QVector<Message> &messages) {
    auto conn = QSqlDatabase::database();

    if (!conn.transaction()) {
        PLOGE << "Could not start transaction";

        return;
    }

    const auto batchId = getNextBatchId(conn);
    saveMessagesAsBatch(conn, messages, batchId);

    conn.commit();
}

qint64
SaveDeviceMessagesCommand::getNextBatchId(const QSqlDatabase &connection) {
    QSqlQuery query { connection };

    if (!query.exec(DML_INSERT_DEVICE_MESSAGE_BATCH)) {
        return 0;
    }

    return query.lastInsertId().toUInt();
}

void
SaveDeviceMessagesCommand::saveMessagesAsBatch(
    const QSqlDatabase& connection,
    const QVector<Message>& messages,
    const qint64 batchId
) {
    const QString POS_BATCH_ID { QStringLiteral(":batch_id") };
    const QString POS_MONKIER  { QStringLiteral(":monkier") };
    const QString POS_PAYLOAD  { QStringLiteral(":payload") };

    QSqlQuery query { connection };

    query.prepare(DML_INSERT_DEVICE_MESSAGE);

    query.bindValue(POS_BATCH_ID, batchId);

    for (const auto& message : messages) {
        query.bindValue(POS_MONKIER, message.moniker);
        query.bindValue(POS_PAYLOAD, message.payload);

        if (!query.exec()) {
            PLOGE << "Could not perform query: " << query.lastError().text();

            return;
        }
    }
}
