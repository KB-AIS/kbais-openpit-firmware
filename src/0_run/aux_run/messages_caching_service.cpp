#include "messages_caching_service.h"

#include "save_device_messages_command.h"
// plog
#include <plog/Log.h>

MessagesCachingService::MessagesCachingService(
    MessagesQueue& msgsQueue, QObject* parent
) : QObject(parent),
    _msgsQueue { msgsQueue },
    _trdWorker { } {
    SaveDeviceMessagesCommand saveDeviceMessagesCommand { };

    _trdWorker.startLoopInThread([&]() {
        DeviceMessage message;

        _msgsQueue.wait_dequeue(message);

        saveDeviceMessagesCommand.execute({ message });
    });
}
