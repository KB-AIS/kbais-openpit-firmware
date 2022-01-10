#ifndef MESSAGES_COLLECTORS_ADAPTER_H
#define MESSAGES_COLLECTORS_ADAPTER_H

// qt
#include <QMutex>
#include <QObject>
// qt deferred
#include <QLambdaThreadWorker>
// oss
#include <readerwriterqueue/readerwriterqueue.h>

#include "messaging/collectors/immediate_messages_collector.h"
#include "messaging/collectors/recurrent_messages_collector.h"
#include "messaging/messages_batch.h"

using MessagesBatchesQueue = moodycamel::BlockingReaderWriterQueue<MessagesBatch>;

class MessagesCollectorsAdapter : public QObject {
    Q_OBJECT

public:
    MessagesCollectorsAdapter(
        ImmediateMessagesCollector& immediateMessagesCollector,
        RecurrentMessagesCollector& recurrentMessagesCollector,
        MessagesBatchesQueue& queue
    );

private:
    QLambdaThreadWorker threadWorker;

    ImmediateMessagesCollector& immediateMessagesCollector;

    RecurrentMessagesCollector& recurrentMessagesCollector;

    MessagesBatchesQueue& queue;

    QMutex mtxCollectMessagses;

    void handleCollectMessages();

};

#endif // MESSAGESCOLLECTORSADAPTER_H
