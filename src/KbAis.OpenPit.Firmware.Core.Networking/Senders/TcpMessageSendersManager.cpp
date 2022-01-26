#include "TcpMessageSendersManager.h"

// std
#include <chrono>
// oss
#include <plog/Log.h>

#include "Networking/Communicators/IProtocolCommunicator.h"
#include "Networking/Communicators/SwomProtocolCommunicator.h"

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds RESTART_MESSAGE_SENDERS_INTERVAL { 5s };

TcpMessageSendersManager::TcpMessageSendersManager(const RxEventBus& eventBus)
    : RxEventModule(eventBus) {
    timerRestartMessageSenders.setInterval(RESTART_MESSAGE_SENDERS_INTERVAL);

    QObject::connect(
        &timerRestartMessageSenders, &QTimer::timeout,

        this, &TcpMessageSendersManager::handleRestartMessageSenders
    );

    subscribe("MESSAGES_BATCH_SAVED", [&](const RxEvent& event) {
        QHash<QUuid, MessageSender*>::iterator itr;
        for (itr = messageSenders.begin(); itr != messageSenders.end(); ++itr) {
           auto senderId = itr.key();

           messageSenders[senderId]->sendMessage();
        }
    });
}

void
TcpMessageSendersManager::handleRestartMessageSenders() {
//    QHash<QUuid, MesssageSenderStatus>::iterator iter;
//    for (iter = messageSenderStatuses.begin(); iter != messageSenderStatuses.end(); ++iter) {
//        auto senderId = iter.key();

//        const auto status = messageSenderStatuses[senderId];

//        const auto hasSenderValidStatus =
//            status.state != SocketState::UnconnectedState;

//        if (hasSenderValidStatus) {
//            continue;
//        }

//        const auto configuration = messageSenderConfigurations[senderId];

//        threadWorker.execInThread([&] {
//            messageSenders[senderId]->restart(configuration);
//        });
//    }
}

void
TcpMessageSendersManager::handleConfigurationChanged(
    const QList<MessageSenderConfiguration>& configurations
) {
//    timerRestartMessageSenders.stop();
//    for (const auto& configuration : configurations) {
//        auto const communicator =
//            QSharedPointer<SwomProtocolCommunicator>::create();
//        threadWorker.moveQObjectToThread(communicator.data());

//        const auto sender = new MessageSender(communicator);
//        threadWorker.moveQObjectToThread(sender);

//        QObject::connect(
//            sender, &MessageSender::notifyStatusChanged,

//            this, &TcpMessageSendersManager::handleMessageSenderStatusChanged
//        );

//        messageSenders[sender->id] = sender;
//        protocolCommunicators[sender->id] = communicator;
//        messageSenderConfigurations[sender->id] = configuration;
//        messageSenderStatuses[sender->id] = {};
//    }
//    timerRestartMessageSenders.start();
}

void
TcpMessageSendersManager::handleMessageSenderStatusChanged(MessageSenderStatusChanged n) {
    messageSenderStatuses[n.senderId].state = n.lastState;
    messageSenderStatuses[n.senderId].error = n.lastError;
}
