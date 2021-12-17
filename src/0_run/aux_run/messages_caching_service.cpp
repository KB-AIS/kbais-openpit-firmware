#include "messages_caching_service.h"

#include "save_device_messages_command.h"
// plog
#include <plog/Log.h>

MessagesCachingService::MessagesCachingService(MessagesQueue& messagesQueue) :
    messagesQueue { messagesQueue },
    threadWorker {} {
    SaveDeviceMessagesCommand saveDeviceMessagesCommand;

    auto handler = [&] {
        DeviceMessage message;
        messagesQueue.wait_dequeue(message);

        saveDeviceMessagesCommand.execute({ message });

        GitlEvent event("MESSAGES_SAVED");
        dispatchEvt(event);
    };

    threadWorker.startLoopInThread(handler);
}
