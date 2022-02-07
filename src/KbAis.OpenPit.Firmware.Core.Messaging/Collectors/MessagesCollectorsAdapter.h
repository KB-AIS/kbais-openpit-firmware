#ifndef MESSAGESCOLLECTORSADAPTER_H
#define MESSAGESCOLLECTORSADAPTER_H

// qt
#include <QObject>
// oss
#include <readerwriterqueue/readerwriterqueue.h>

// Utils.TrdParty.RxQt
#include "RxQt.h"

#include "Collectors/ImmediateMessagesCollector.h"
#include "Collectors/RecurrentMessagesCollector.h"
#include "MessagesBatch.h"

class MessagesCollectorsAdapter : public QObject {
    Q_OBJECT

    using MessagesBatchesCachingQueue_t = moodycamel::BlockingReaderWriterQueue<MessagesBatch>;

public:
    MessagesCollectorsAdapter(
        ImmediateMessagesCollector& immediateMessagesCollector
    ,   RecurrentMessagesCollector& recurrentMessagesCollector
    ,   MessagesBatchesCachingQueue_t& queue
    );

    ~MessagesCollectorsAdapter() noexcept;

    void StartCollectingOn(const rxcpp::observe_on_one_worker& scheduler);

private:
    ImmediateMessagesCollector& mImmediateMessagesCollector;

    RecurrentMessagesCollector& mRecurrentMessagesCollector;

    rxcpp::composite_subscription mSubs;

    MessagesBatchesCachingQueue_t& mQueue;

    void handleCollectMessages();

};

#endif // MESSAGESCOLLECTORSADAPTER_H
