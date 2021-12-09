#ifndef MESSAGESCACHINGSERVICE_H
#define MESSAGESCACHINGSERVICE_H

#include "device_message.h"
#include "messages_sending_service.h"
// Qt
#include <QObject>
// Qt Deferred
#include <QDeferred>
#include <QEventer>
#include <QLambdaThreadWorker>
// OSS
#include <rigtorp/MPMCQueue.h>

using MessagesQueue = rigtorp::mpmc::Queue<DeviceMessage>;

class MessagesCachingService : public QObject {
    Q_OBJECT

public:
    explicit MessagesCachingService(
        MessagesQueue& msgsQueue, MessagesSendingService& msgsSendingSrv,
        QObject* = nullptr);

private:
    MessagesQueue& _msgsQueue;

    MessagesSendingService& _msgsSendingSrv;

    QLambdaThreadWorker _trdWorker;

};

#endif // MESSAGESCACHINGSERVICE_H
