#include "messages_caching_service.h"

MessagesCachingService::MessagesCachingService(MessagesBatchesQueue& queue) :
    queue { queue }, threadWorker { } {


    auto handler = [&] {
        MessagesBatch messagesBatch;

        // Blockingly get the next messages batch.
        queue.wait_dequeue(messagesBatch);

        saveMessagesBatchCommand.handle(messagesBatch);

        dispatchMessagesBatchCreated();
    };

    threadWorker.startLoopInThread(handler);
}

void
MessagesCachingService::dispatchMessagesBatchCreated() {
    GitlEvent event("MESSAGES_BATCH_SAVED");
    dispatchEvt(event);
}
