#include "BlockingMessagesCachingService.h"

// oss
#include <fmt/core.h>
#include <fmt/format.h>
#include <plog/Log.h>

BlockingMessagesCachingService::BlockingMessagesCachingService(
    MessagesBatchQueue& messagesBatchQueue, const RxEventBus& eventBus
)
    : RxEventModule(eventBus)
    , messagesBatchQueue { messagesBatchQueue }
    , eventBus { eventBus }
{
    subMessagesBatcheQueue = rxcpp::composite_subscription();

    auto messagesBatchesQueueObservable = rxcpp::observable<>
        ::create<MessagesBatch>([&](rxcpp::subscriber<MessagesBatch> x) {
            MessagesBatch messagesBatch;
            try {
                forever {
                    messagesBatchQueue.wait_dequeue(messagesBatch);

                    x.on_next(messagesBatch);
                }
            } catch (...) {
                x.on_error(std::current_exception());
            }
            x.on_completed();
        })
        .subscribe_on(rxcpp::synchronize_new_thread());

    messagesBatchesQueueObservable.subscribe(
        subMessagesBatcheQueue
    ,   [&](const MessagesBatch& x) {
            PLOGV << "Messages caching service got a new batch";

            insertMessagesBatchCmd.handle(x);

            postEventMessagesBatchSaved();
        }
    ,   [](std::exception_ptr e){
            PLOGE << fmt::format("Error on messages batch caching: {}", rxcpp::util::what(e));
        }
    );
}

BlockingMessagesCachingService::~BlockingMessagesCachingService() {
    subMessagesBatcheQueue.unsubscribe();
}

void
BlockingMessagesCachingService::postEventMessagesBatchSaved() const {
    eventBus.post(RxEvent { "MESSAGES_BATCH_SAVED" });
}
