#ifndef MESSAGESCOLLECTORSADAPTER_H
#define MESSAGESCOLLECTORSADAPTER_H

// qt
#include <QMutex>
#include <QObject>
// oss
#include <readerwriterqueue/readerwriterqueue.h>

// trdparty::rxqt
#include "RxQt.h"

#include "Messaging/Collectors/RecurrentMessagesCollector.h"

//using MessagesBatchesQueue = moodycamel::BlockingReaderWriterQueue<MessagesBatch>;

class MessagesCollectorsAdapter : public QObject {
    Q_OBJECT

public:
    MessagesCollectorsAdapter(
        //ImmediateMessagesCollector& immediateMessagesCollector,
        RecurrentMessagesCollector& recurrentMessagesCollector/*,
        MessagesBatchesQueue& queue*/
    );

private:
    //ImmediateMessagesCollector& immediateMessagesCollector;

    RecurrentMessagesCollector& recurrentMessagesCollector;

    rxcpp::composite_subscription subs;

    //MessagesBatchesQueue& queue;

    QMutex mtxCollectMessagses;

    void handleCollectMessages();

};

#endif // MESSAGESCOLLECTORSADAPTER_H
