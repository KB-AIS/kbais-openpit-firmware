#include "get_message_batches_query.h"

// Qt
#include <QMap>
#include <QVariant>
// Qt SQL
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
// plog
#include <plog/Log.h>

using namespace kbais::cfw::msgcaching;

QString QML_SELECT_DEVICE_MESSAGE_BATCHES { QStringLiteral(
    "SELECT"
    "   [dmb].[id] as deviceMessageBatch_id,"
    "   [dm].[monkier] as deviceMessage_monkier,"
    "   [dm].[payload] as deviceMessage_payload"
    "FROM"
    "   [device_message_batches] AS [dmb]"
    "LEFT JOIN"
    "   [device_messages] AS [dm]"
    "       ON [dmb].[id] = [dm].[batch_id]"
    "ORDER BY"
    "   [dmb].[id] DESC"
    "LIMIT"
    "   :batch_count;"
) };

QList<DeviceMessageBatch>
GetMessageBatchesQueryHandler::query(qint32 batchCount) const {
    auto connection = QSqlDatabase::database();

    QSqlQuery query { connection };

    query.prepare(QML_SELECT_DEVICE_MESSAGE_BATCHES);

    query.bindValue(":batch_count", batchCount);

    // TODO: Process error with result monodik
    if (!query.exec()) {
        PLOGE << "Could not perform query: " << query.lastError().text();

        return { };
    }

    QMap<quint64, DeviceMessageBatch> batches;
    while (query.next()) {
        auto batchId = query.value("deviceMessageBatch_id").toULongLong();

        if (!batches.contains(batchId)) {
            batches[batchId] = { batchId, QDateTime::currentDateTime(), { } };
        }

        batches[batchId].messages.push_back({
            query.value("deviceMessage_monkier").toString(),
            query.value("deviceMessage_payload").toByteArray()
        });
    }

    return batches.values();
}


