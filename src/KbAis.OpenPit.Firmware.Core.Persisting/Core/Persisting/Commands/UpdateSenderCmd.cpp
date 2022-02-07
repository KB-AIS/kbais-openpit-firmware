#include "UpdateSenderCmd.h"

// qt
#include <QVariant>
// qt sql
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
// oss
#include <plog/Log.h>

const QString DML_UPDATE_SENDER { QStringLiteral(
    "UPDATE [senders]\n"
    "SET\n"
    "   [last_sent_messages_batch_id] = :messages_batch_id\n"
    "WHERE\n"
    "   [host] = '10.214.1.208' AND [port] = 9900;"
) };

void UpdateSenderCmd::handle(quint64 messagesBatchId) const {
    auto connection = QSqlDatabase::database();

    QSqlQuery query { connection };

    query.prepare(DML_UPDATE_SENDER);
    query.bindValue(":messages_batch_id", messagesBatchId);

    if (!query.exec()) {
        PLOGE << "Could not update sender: {0}" << query.lastError().text();
    }
}
