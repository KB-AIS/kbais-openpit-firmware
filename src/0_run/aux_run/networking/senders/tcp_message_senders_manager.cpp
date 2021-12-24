#include "tcp_message_senders_manager.h"

// std
#include <chrono>
// oss
#include <plog/Log.h>

#include "networking/communicators/swom_protocol_communicator.h"
#include "networking/communicators/base_protocol_communicator.h"

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds RESTART_MESSAGE_SENDERS_INTERVAL { 5s };

TcpMessageSendersManager::TcpMessageSendersManager() {
    restartMessageSendersTimer.setInterval(RESTART_MESSAGE_SENDERS_INTERVAL);

    QObject::connect(
        &restartMessageSendersTimer, &QTimer::timeout,

        this, &TcpMessageSendersManager::handleRestartMessageSenders
    );

    subscribeToEvtByName("MESSAGES_BATCH_SAVED", [&](auto event)->bool {
        QHash<QUuid, MessageSender*>::iterator iter;
        for (iter = messageSenders.begin(); iter != messageSenders.end(); ++iter) {
            auto senderId = iter.key();

            messageSenders[senderId]->sendMessage();
        }
    });
}

void
TcpMessageSendersManager::handleRestartMessageSenders() {
    QHash<QUuid, MesssageSenderStatus>::iterator iter;
    for (iter = messageSenderStatuses.begin(); iter != messageSenderStatuses.end(); ++iter) {
        auto senderId = iter.key();

        const auto status = messageSenderStatuses[senderId];

        const auto hasSenderValidStatus =
            status.state != SocketState::UnconnectedState;

        if (hasSenderValidStatus) {
            continue;
        }

        const auto configuration = messageSenderConfigurations[senderId];

        threadWorker.execInThread([&] {
            messageSenders[senderId]->restart(configuration);
        });
    }
}

void
TcpMessageSendersManager::handleConfigurationChanged(
    const QList<MessageSenderConfiguration>& configurations
) {
    restartMessageSendersTimer.stop();
    for (const auto& configuration : configurations) {
        auto const communicator =
            QSharedPointer<SwomProtocolCommunicator>::create();
        threadWorker.moveQObjectToThread(communicator.data());

        const auto sender = new MessageSender(communicator);
        threadWorker.moveQObjectToThread(sender);

        QObject::connect(
            sender, &MessageSender::notifyStatusChanged,

            this, &TcpMessageSendersManager::handleMessageSenderStatusChanged
        );

        messageSenders[sender->id] = sender;
        protocolCommunicators[sender->id] = communicator;
        messageSenderConfigurations[sender->id] = configuration;
        messageSenderStatuses[sender->id] = {};
    }
    restartMessageSendersTimer.start();
}

void
TcpMessageSendersManager::handleMessageSenderStatusChanged(MessageSenderStatusChanged n) {
    messageSenderStatuses[n.senderId].state = n.lastState;
    messageSenderStatuses[n.senderId].error = n.lastError;
}
