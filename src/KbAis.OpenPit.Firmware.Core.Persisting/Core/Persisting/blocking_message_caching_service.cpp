#include "blocking_message_caching_service.h"

// oss
#include <fmt/format.h>
#include <plog/Log.h>

blocking_message_caching_service::blocking_message_caching_service(
    MessagesBatchQueue_t& messagesBatchQueue
,   const RxEventBus& eventBus
)
    :   RxEventModule(eventBus)
    ,   m_messagesBatchQueue(messagesBatchQueue)
    ,   m_eventBus(eventBus)
{
    m_messagesBatchesQueueObservable = rxcpp::observable<>
        ::create<MessagesBatch>([&q = m_messagesBatchQueue](rxcpp::subscriber<MessagesBatch> x) {
            MessagesBatch messagesBatch;
            try {
                forever {
                    q.wait_dequeue(messagesBatch);

                    x.on_next(messagesBatch);
                }
            } catch (...) {
                x.on_error(std::current_exception());
            }
            x.on_completed();
        });
}

blocking_message_caching_service::~blocking_message_caching_service() {
    m_subscriptions.unsubscribe();
}

void
blocking_message_caching_service::start_working_on(const rxcpp::observe_on_one_worker& coordination) {
    m_subscriptions = rxcpp::composite_subscription();

    m_messagesBatchesQueueObservable
        .subscribe_on(coordination)
        .subscribe(
            m_subscriptions
        ,   [&](const MessagesBatch& x) {
                PLOGV << "Messages caching service got a new batch";

                m_insertMessagesBatchCmd.handle(x);

                postEventMessagesBatchSaved();
            }
        ,   [](std::exception_ptr e){
                PLOGE << fmt::format("Error on messages batch caching: {}", rxcpp::util::what(e));
            }
        );
}

void
blocking_message_caching_service::postEventMessagesBatchSaved() const {
    m_eventBus.post(RxEvent { "MESSAGES_BATCH_SAVED" });
}
