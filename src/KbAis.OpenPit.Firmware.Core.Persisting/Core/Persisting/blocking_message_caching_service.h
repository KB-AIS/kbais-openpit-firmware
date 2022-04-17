#ifndef BLOCKING_MESSAGE_CACHING_SERVICE_H
#define BLOCKING_MESSAGE_CACHING_SERVICE_H

// oss
#include <readerwriterqueue/readerwriterqueue.h>

#include "RxEventBus.h"
#include "MessagesBatch.h"
#include "Core/Persisting/i_message_caching_service.h"
#include "Core/Persisting/Commands/InsertMessagesBatchCmd.h"

class blocking_message_caching_service : public i_message_caching_service, public RxEventModule {

using MessagesBatchQueue_t = moodycamel::BlockingReaderWriterQueue<MessagesBatch>;

    MessagesBatchQueue_t& m_messagesBatchQueue;

    const RxEventBus& m_eventBus;

    rxcpp::composite_subscription m_subscriptions;

    rxcpp::observable<MessagesBatch> m_messagesBatchesQueueObservable;

    InsertMessagesBatchCmd m_insertMessagesBatchCmd;

    void postEventMessagesBatchSaved() const;

public:
    blocking_message_caching_service(
        MessagesBatchQueue_t& messagesBatchQueue, const RxEventBus& eventBus
    );

    ~blocking_message_caching_service();

    void start_working_on(const rxcpp::observe_on_one_worker& coordination) override;

};

#endif // BLOCKING_MESSAGE_CACHING_SERVICE_H
