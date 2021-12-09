#include "messages_caching_service.h"

// plog
#include <plog/Log.h>
#include <QThread>

MessagesCachingService::MessagesCachingService(
    MessagesQueue& msgsQueue, MessagesSendingService& msgsSendingSrv,
    QObject* parent
) : QObject(parent),
    _msgsQueue { msgsQueue },
    _msgsSendingSrv { msgsSendingSrv },
    _trdWorker { } {

    _trdWorker.startLoopInThread([&]() {
        DeviceMessage msg;

        _msgsQueue.pop(msg);

        _msgsSendingSrv.sendMessages({ msg });
    });

}
