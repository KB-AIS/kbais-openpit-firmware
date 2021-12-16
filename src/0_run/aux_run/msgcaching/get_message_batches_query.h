#ifndef GET_MESSAGE_BATCHES_QUERY_H
#define GET_MESSAGE_BATCHES_QUERY_H

#include "device_message.h"
// Qt
#include <QList>

namespace KbAis::Cfw::DatabaseCaching {

class GetMessageBatchesQueryHandler {

public:
    QList<DeviceMessageBatch> query(qint32 batchCount) const;

};

}

#endif // GET_MESSAGE_BATCHES_QUERY_H
