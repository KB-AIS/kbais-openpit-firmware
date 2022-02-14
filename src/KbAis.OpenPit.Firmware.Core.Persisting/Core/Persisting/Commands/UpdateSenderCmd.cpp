#include "UpdateSenderCmd.h"

// qt
#include <QSqlDatabase>
#include <QSqlError>
#include <QVariant>
// oss
#include <fmt/format.h>
#include <plog/Log.h>

const QString DML_UPDATE_SENDER { QStringLiteral(
    "UPDATE [senders]\n"
    "SET\n"
    "   [last_sent_messages_batch_id] = :messages_batch_id\n"
    "WHERE\n"
    "   [host] = '10.214.1.208' AND [port] = 9900;"
) };

MessageSenderUpdateCmd::MessageSenderUpdateCmd() {

}

MessageSenderUpdateCmd::~MessageSenderUpdateCmd() {
    m_qryUpdateSenders.finish();
}

void MessageSenderUpdateCmd::handle(quint64 messagesBatchId) const {
    auto connection = QSqlDatabase::database();

    QSqlQuery m_qryUpdateSenders { connection };

    m_qryUpdateSenders.prepare(DML_UPDATE_SENDER);
    m_qryUpdateSenders.bindValue(":messages_batch_id", messagesBatchId);

    if (!m_qryUpdateSenders.exec()) {
        const auto error = m_qryUpdateSenders.lastError().text().toStdString();

        PLOGE << fmt::format("Could not update sender: {}", error);
    }
}
