#ifndef GET_MESSAGES_BATCHES_QUERY_H
#define GET_MESSAGES_BATCHES_QUERY_H

// qt
#include <QList>

#include "messaging/message.h"
#include "messaging/messages_batch.h"

class GetMessagesBatchesQuery {

public:
    QList<MessagesBatch> handle(qint32 count) const;

};

#endif // GET_MESSAGES_BATCHES_QUERY_H
