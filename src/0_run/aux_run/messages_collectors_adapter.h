#ifndef MESSAGESCOLLECTORSADAPTER_H
#define MESSAGESCOLLECTORSADAPTER_H

#include "immediate_messages_collector.h"
#include "recurrent_messages_collector.h"
// Qt
#include <QMutex>
#include <QObject>
// QDeferred
#include <QLambdaThreadWorker>
// OSS
#include <rigtorp/MPMCQueue.h>
#include <readerwriterqueue.h>

using namespace moodycamel;

using MessagesQueue = rigtorp::mpmc::Queue<DeviceMessage>;

class MessagesCollectorsAdapter : public QObject {
    Q_OBJECT

public:
    explicit MessagesCollectorsAdapter(
        RecurrentMessagesCollector& recurrentCollector,
        ImmediateMessagesCollector& immediateCollector,
        BlockingReaderWriterQueue<DeviceMessage>& msgsQueue,
        QObject* parent = nullptr
    );

    Q_SIGNAL void notifyMessagesCollected();

private:
    void collectMessages();

    QLambdaThreadWorker _trdWorker;

    RecurrentMessagesCollector& recurrentCollector;

    ImmediateMessagesCollector& immediateCollector;

    BlockingReaderWriterQueue<DeviceMessage>& _msgsQueue;

    QMutex collectMsgsMtx;

};

#endif // MESSAGESCOLLECTORSADAPTER_H
