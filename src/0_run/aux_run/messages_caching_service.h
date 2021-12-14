#ifndef MESSAGESCACHINGSERVICE_H
#define MESSAGESCACHINGSERVICE_H

#include "device_message.h"
#include "networking/message_sender.h"
// Qt
#include <QObject>
// Qt Deferred
#include <QDeferred>
#include <QEventer>
#include <QLambdaThreadWorker>
// OSS
#include <readerwriterqueue.h>

using MessagesQueue = moodycamel::BlockingReaderWriterQueue<DeviceMessage>;

class MessagesCachingService : public QObject {
    Q_OBJECT

public:
    explicit MessagesCachingService(MessagesQueue& msgsQueue, QObject* = nullptr);

private:
    MessagesQueue& _msgsQueue;

    QLambdaThreadWorker _trdWorker;

};

#endif // MESSAGESCACHINGSERVICE_H
