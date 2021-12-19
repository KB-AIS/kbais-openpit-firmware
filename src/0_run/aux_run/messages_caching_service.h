#ifndef MESSAGESCACHINGSERVICE_H
#define MESSAGESCACHINGSERVICE_H

// qt
#include <QObject>
// qt defferred
#include <QDeferred>
#include <QEventer>
#include <QLambdaThreadWorker>
// oss
#include <readerwriterqueue.h>
#include <gitlmodule.h>

#include "device_message.h"
#include "networking/message_sender.h"

using MessagesQueue = moodycamel::BlockingReaderWriterQueue<Message>;

class MessagesCachingService : public QObject, public GitlModule {
    Q_OBJECT

public:
    explicit MessagesCachingService(MessagesQueue& msgsQueue);

private:
    MessagesQueue& messagesQueue;

    QLambdaThreadWorker threadWorker;

};

#endif // MESSAGESCACHINGSERVICE_H
