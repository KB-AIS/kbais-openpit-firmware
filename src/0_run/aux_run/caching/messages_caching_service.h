#ifndef MESSAGES_CACHING_SERVICE_H
#define MESSAGES_CACHING_SERVICE_H

// qt
#include <QObject>
// qt defferred
#include <QDeferred>
#include <QLambdaThreadWorker>
// oss
#include <readerwriterqueue/readerwriterqueue.h>

#include "persisting/commands/save_messages_batch_command.h"
#include "messaging/messages_batch.h"

class IMessagesCachingService : public QObject {

public:
    virtual ~IMessagesCachingService() noexcept = default;
};

class MessagesCachingService : public IMessagesCachingService {
    Q_OBJECT

public:
    MessagesCachingService(moodycamel::BlockingReaderWriterQueue<MessagesBatch>& queue);

private:
    moodycamel::BlockingReaderWriterQueue<MessagesBatch>& queue;

    QLambdaThreadWorker threadWorker;

    SaveMessagesBatchCommand saveMessagesBatchCommand;

};

#endif // MESSAGES_CACHING_SERVICE_H
