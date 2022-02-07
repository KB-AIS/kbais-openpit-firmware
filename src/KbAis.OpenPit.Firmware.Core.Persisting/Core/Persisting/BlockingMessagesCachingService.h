#ifndef BLOCKINGMESSAGESCACHINGSERVICE_H
#define BLOCKINGMESSAGESCACHINGSERVICE_H

// oss
#include <readerwriterqueue/readerwriterqueue.h>

// cfw::infra::eventbus
#include "RxEventBus.h"

#include "MessagesBatch.h"

#include "Core/Persisting/IMessagesCachingService.h"
#include "Core/Persisting/Commands/InsertMessagesBatchCmd.h"

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

    void StartWorkOn(rxcpp::observe_on_one_worker& coordination);

private:
    MessagesBatchQueue_t& m_messagesBatchQueue;

    const RxEventBus& m_eventBus;

    rxcpp::composite_subscription m_subscriptions;

    rxcpp::observable<MessagesBatch> m_messagesBatchesQueueObservable;

    InsertMessagesBatchCmd m_insertMessagesBatchCmd;

    void postEventMessagesBatchSaved() const;
};

#endif // BLOCKINGMESSAGESCACHINGSERVICE_H
