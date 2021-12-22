#ifndef SAVE_MESSAGES_BATCH_COMMAND_H
#define SAVE_MESSAGES_BATCH_COMMAND_H

// qt
#include <QVector>
#include <QPair>
// qt sql
#include <QSqlDatabase>
#include <QSqlQuery>

#include "messaging/message.h"
#include "messaging/messages_batch.h"

class SaveMessagesBatchCommand {

public:
    void handle(const MessagesBatch& messagesBatch);

private:
    QPair<bool, quint32> insertMessagesBatch(
        const QSqlDatabase &connection,
        const MessagesBatch& messagesBatch
    );

    bool insertMessages(
        const QSqlDatabase& connection,
        const qint64 messagesBatchId,
        const QVector<Message>& messages
    );

};

#endif // SAVE_MESSAGES_BATCH_COMMAND_H
