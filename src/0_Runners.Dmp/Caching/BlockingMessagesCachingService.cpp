#include "BlockingMessagesCachingService.h"

using MessagesBatchesQueue = moodycamel::BlockingReaderWriterQueue<MessagesBatch>;

BlockingMessagesCachingService::BlockingMessagesCachingService(
    MessagesBatchesQueue& messagesBatchQueue, RxEventBus& eventBus
)
    : RxEventModule(eventBus)
    , messagesBatchQueue { messagesBatchQueue }
    , eventBus { eventBus }
{
    auto messagesBatchesQueueObserver = rxcpp::observable<>
        ::create<MessagesBatch>([&](rxcpp::subscriber<MessagesBatch> x) {
            MessagesBatch messagesBatch;

            // Blockingly get the next messages batch.
            messagesBatchQueue.wait_dequeue(messagesBatch);

            x.on_next(messagesBatch);
        })
        .subscribe_on(rxcpp::observe_on_new_thread());

    messagesBatchesQueueObserver.subscribe([&](const MessagesBatch& x) {
        // saveMessagesBatchCommand.handle(x);

        publishEventMessagesBatchSaved();
    });
}

void
BlockingMessagesCachingService::publishEventMessagesBatchSaved() const {
    eventBus.post(RxEvent { "MESSAGES_BATCH_SAVED" });
}
