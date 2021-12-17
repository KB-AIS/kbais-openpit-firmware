#ifndef SAVEDEVICEMESSAGESCOMMAND_H
#define SAVEDEVICEMESSAGESCOMMAND_H

#include "database_configuration.h"
#include "device_message.h"
// Qt
#include <QVector>
// Qt SQL
#include <QSqlDatabase>
#include <QSqlQuery>

class SaveDeviceMessagesCommand {

public:
    void execute(const QVector<DeviceMessage>& msgs);

private:
    qint64 getNextBatchId(const QSqlDatabase& connection);

    void saveMessagesAsBatch(
        const QSqlDatabase& connection,
        const QVector<DeviceMessage>& messages,
        const qint64 batchId
    );

    static void bindMessage(QSqlQuery& query, const DeviceMessage& message);

};

#endif // SAVEDEVICEMESSAGESCOMMAND_H
