#ifndef MESSAGES_CACHING_SERVICE_H
#define MESSAGES_CACHING_SERVICE_H

// qt
#include <QObject>
// qt defferred
#include <QDeferred>
#include <QLambdaThreadWorker>
// oss
#include <gitlmodule.h>
#include <readerwriterqueue/readerwriterqueue.h>

#include "database/commands/save_messages_batch_command.h"
#include "messaging/messages_batch.h"

using MessagesBatchesQueue = moodycamel::BlockingReaderWriterQueue<MessagesBatch>;

class MessagesCachingService : public QObject, public GitlModule {
    Q_OBJECT

public:
    explicit MessagesCachingService(MessagesBatchesQueue& queue);

private:
    MessagesBatchesQueue& queue;

    QLambdaThreadWorker threadWorker;

    SaveMessagesBatchCommandHandler saveMessagesBatchCommand;

    void dispatchMessagesBatchCreated();

};

#endif // MESSAGES_CACHING_SERVICE_H
