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
#include "RxEventBus.h"

class IMessagesCachingService : public QObject {

public:
    virtual ~IMessagesCachingService() noexcept = default;

};

using  MessagesBatchQueue = moodycamel::BlockingReaderWriterQueue<MessagesBatch>;

class MessagesCachingService : public IMessagesCachingService, public RxEventModule {
    Q_OBJECT

public:
    MessagesCachingService(MessagesBatchQueue& queue, RxEventBus& bus);

private:
    MessagesBatchQueue& messagesBatchQueue;

    RxEventBus& eventBus;

    QLambdaThreadWorker threadWorker;

    SaveMessagesBatchCommand saveMessagesBatchCommand;

    void publishMessagesBatchSaved();
};

#endif // MESSAGES_CACHING_SERVICE_H
