#ifndef MESSAGESCOLLECTORSADAPTER_H
#define MESSAGESCOLLECTORSADAPTER_H

// qt
#include <QObject>
// oss
#include <readerwriterqueue/readerwriterqueue.h>

// cfw::trdparty
#include "RxQt/RxQt.h"

#include "Messaging/Collectors/ImmediateMessagesCollector.h"
#include "Messaging/Collectors/RecurrentMessagesCollector.h"
#include "Messaging/MessagesBatch.h"

using MessagesBatchesQueue = moodycamel::BlockingReaderWriterQueue<MessagesBatch>;

class MessagesCollectorsAdapter : public QObject {
    Q_OBJECT

public:
    MessagesCollectorsAdapter(
        ImmediateMessagesCollector& immediateMessagesCollector,
        RecurrentMessagesCollector& recurrentMessagesCollector,
        MessagesBatchesQueue& queue
    );

    ~MessagesCollectorsAdapter();

private:
    ImmediateMessagesCollector& immediateMessagesCollector;

    RecurrentMessagesCollector& recurrentMessagesCollector;

    rxcpp::composite_subscription subs;

    MessagesBatchesQueue& queue;

    void handleCollectMessages();

};

#endif // MESSAGESCOLLECTORSADAPTER_H
