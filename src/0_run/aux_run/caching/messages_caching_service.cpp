#include "messages_caching_service.h"

using MessagesBatchesQueue = moodycamel::BlockingReaderWriterQueue<MessagesBatch>;

MessagesCachingService::MessagesCachingService(MessagesBatchesQueue& queue) :
    queue { queue }, threadWorker { } {
    auto handleNextMessagesBatch = [&] {
        MessagesBatch messagesBatch;

        // Blockingly get the next messages batch.
        queue.wait_dequeue(messagesBatch);

        saveMessagesBatchCommand.handle(messagesBatch);
    };

    threadWorker.startLoopInThread(handleNextMessagesBatch);
}
