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

class IMessagesCachingService : public QObject {

public:
    virtual ~IMessagesCachingService() noexcept = default;
};

class MessagesCachingService : public IMessagesCachingService, public GitlModule {
    Q_OBJECT

public:
    MessagesCachingService(moodycamel::BlockingReaderWriterQueue<MessagesBatch>& queue);

private:
    moodycamel::BlockingReaderWriterQueue<MessagesBatch>& queue;

    QLambdaThreadWorker threadWorker;

    SaveMessagesBatchCommand saveMessagesBatchCommand;

    void dispatchMessagesBatchCreated();

};

#endif // MESSAGES_CACHING_SERVICE_H
