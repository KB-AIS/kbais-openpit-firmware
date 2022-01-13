#ifndef BLOCKINGMESSAGESCACHINGSERVICE_H
#define BLOCKINGMESSAGESCACHINGSERVICE_H

// oss
#include <readerwriterqueue/readerwriterqueue.h>

// cfw::infra::eventbus
#include "RxEventBus.h"

// #include "persisting/commands/save_messages_batch_command.h"
#include "Caching/IMessagesCachingService.h"
#include "Messaging/MessagesBatch.h"

class BlockingMessagesCachingService : public IMessagesCachingService, public RxEventModule {

public:
    BlockingMessagesCachingService(
        moodycamel::BlockingReaderWriterQueue<MessagesBatch>& messagesBatchQueue,
        RxEventBus& eventBus
    );

private:
    moodycamel::BlockingReaderWriterQueue<MessagesBatch>& messagesBatchQueue;

    RxEventBus& eventBus;

    // SaveMessagesBatchCommand saveMessagesBatchCommand;

    void publishEventMessagesBatchSaved() const;
};

#endif // BLOCKINGMESSAGESCACHINGSERVICE_H
