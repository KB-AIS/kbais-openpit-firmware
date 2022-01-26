#ifndef BLOCKINGMESSAGESCACHINGSERVICE_H
#define BLOCKINGMESSAGESCACHINGSERVICE_H

// oss
#include <readerwriterqueue/readerwriterqueue.h>

// cfw::infra::eventbus
#include "RxEventBus.h"

#include "Caching/IMessagesCachingService.h"
#include "Messaging/MessagesBatch.h"
#include "Persisting/Commands/InsertMessagesBatchCmd.h"

/**
 * \brief Реализация сервиса кеширования сообщений использующая
 * блокирующую очередь в собственном потоке
 */
class BlockingMessagesCachingService : public IMessagesCachingService, public RxEventModule {

using MessagesBatchQueue = moodycamel::BlockingReaderWriterQueue<MessagesBatch>;

public:
    BlockingMessagesCachingService(
        MessagesBatchQueue& messagesBatchQueue, const RxEventBus& eventBus
    );

    ~BlockingMessagesCachingService();

private:
    rxcpp::composite_subscription subMessagesBatcheQueue;

    MessagesBatchQueue& messagesBatchQueue;

    const RxEventBus& eventBus;

    InsertMessagesBatchCmd insertMessagesBatchCmd;

    void postEventMessagesBatchSaved() const;
};

#endif // BLOCKINGMESSAGESCACHINGSERVICE_H
