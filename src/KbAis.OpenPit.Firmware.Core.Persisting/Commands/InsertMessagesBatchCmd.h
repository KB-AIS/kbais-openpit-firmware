#ifndef INSERTMESSAGESBATCHCMD_H
#define INSERTMESSAGESBATCHCMD_H

// qt
#include <QVector>
#include <QPair>
// qt sql
#include <QSqlDatabase>
#include <QSqlQuery>

#include "Messaging/Message.h"
#include "Messaging/MessagesBatch.h"

class InsertMessagesBatchCmd {

public:
    // TODO: add sender
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

#endif // INSERTMESSAGESBATCHCMD_H
