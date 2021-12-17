#ifndef MESSAGESCOLLECTORSADAPTER_H
#define MESSAGESCOLLECTORSADAPTER_H

// qt
#include <QMutex>
#include <QObject>
// qt deferred
#include <QLambdaThreadWorker>
// oss
#include <readerwriterqueue.h>

#include "immediate_messages_collector.h"
#include "recurrent_messages_collector.h"

class MessagesCollectorsAdapter : public QObject {
    Q_OBJECT

public:
    explicit MessagesCollectorsAdapter(
        RecurrentMessagesCollector& recurrentCollector,
        ImmediateMessagesCollector& immediateCollector,
        moodycamel::BlockingReaderWriterQueue<DeviceMessage>& messagesQueue,
        QObject* parent = nullptr
    );

    Q_SIGNAL void notifyMessagesCollected();

private:
    void collectMessages();

    QLambdaThreadWorker threadWorker;

    RecurrentMessagesCollector& recurrentCollector;

    ImmediateMessagesCollector& immediateCollector;

    moodycamel::BlockingReaderWriterQueue<DeviceMessage>& messagesQueue;

    QMutex collectMsgsMtx;

};

#endif // MESSAGESCOLLECTORSADAPTER_H
