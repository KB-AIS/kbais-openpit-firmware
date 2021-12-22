#include "messages_caching_service.h"

using MessagesBatchesQueue = moodycamel::BlockingReaderWriterQueue<MessagesBatch>;

MessagesCachingService::MessagesCachingService(MessagesBatchesQueue& queue) :
    queue { queue }, threadWorker { } {
    auto handleNextMessagesBatch = [&] {
        MessagesBatch messagesBatch;

        // Blockingly get the next messages batch.
        queue.wait_dequeue(messagesBatch);

        saveMessagesBatchCommand.handle(messagesBatch);

        dispatchMessagesBatchCreated();
    };

    threadWorker.startLoopInThread(handleNextMessagesBatch);
}

void
MessagesCachingService::dispatchMessagesBatchCreated() {
    GitlEvent event("MESSAGES_BATCH_SAVED");

    dispatchEvt(event);
}
