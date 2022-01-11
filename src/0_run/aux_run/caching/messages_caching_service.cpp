#include "messages_caching_service.h"

using MessagesBatchesQueue = moodycamel::BlockingReaderWriterQueue<MessagesBatch>;

MessagesCachingService::MessagesCachingService(
    MessagesBatchesQueue& queue, RxEventBus& bus
)
    : RxEventModule(bus)
    , messagesBatchQueue { queue }
    , eventBus { bus } {
    auto handleNextMessagesBatch = [&] {
        MessagesBatch messagesBatch;

        // Blockingly get the next messages batch.
        queue.wait_dequeue(messagesBatch);

        saveMessagesBatchCommand.handle(messagesBatch);

        publishMessagesBatchSaved();
    };

    threadWorker.startLoopInThread(handleNextMessagesBatch);
}

void
MessagesCachingService::publishMessagesBatchSaved() {
    eventBus.post(RxEvent { "MESSAGES_BATCH_SAVED" });
}
