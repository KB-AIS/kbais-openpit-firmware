#ifndef BLOCKINGMESSAGESCACHINGSERVICE_H
#define BLOCKINGMESSAGESCACHINGSERVICE_H

// oss
#include <readerwriterqueue/readerwriterqueue.h>

// cfw::infra::eventbus
#include "RxEventBus.h"

#include "IMessagesCachingService.h"
#include "MessagesBatch.h"
#include "Commands/InsertMessagesBatchCmd.h"

class BlockingMessagesCachingService
    :   public IMessagesCachingService
    ,   public RxEventModule
{

using MessagesBatchQueue_t = moodycamel::BlockingReaderWriterQueue<MessagesBatch>;

public:
    BlockingMessagesCachingService(
        MessagesBatchQueue_t& messagesBatchQueue, const RxEventBus& eventBus
    );

    ~BlockingMessagesCachingService();

private:
    rxcpp::composite_subscription subMessagesBatcheQueue;

    MessagesBatchQueue_t& messagesBatchQueue;

    const RxEventBus& eventBus;

    InsertMessagesBatchCmd insertMessagesBatchCmd;

    void postEventMessagesBatchSaved() const;
};

#endif // BLOCKINGMESSAGESCACHINGSERVICE_H
