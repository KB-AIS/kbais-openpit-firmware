#include "message_senders_manager.h"

// std
#include <chrono>

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds RESTART_MESSAGE_SENDERS_INTERVAL { 5s };

TcpMessageSendersManager::TcpMessageSendersManager() {
    restartMessageSendersTimer.setInterval(RESTART_MESSAGE_SENDERS_INTERVAL);

    QObject::connect(
        &restartMessageSendersTimer, &QTimer::timeout,

        this, &TcpMessageSendersManager::handleRestartMessageSenders
    );

    restartMessageSendersTimer.start();
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
        messageSenders[senderId]->restart(configuration);
    }
}

void
TcpMessageSendersManager::handleConfigurationChanged(
    const QList<MessageSenderConfiguration>& configurations
) {
    for (const auto& configuration : configurations) {
        const auto sender = new MessageSender();

        QObject::connect(
            sender, &MessageSender::notifyStatusChanged,

            this, &TcpMessageSendersManager::handleMessageSenderStatusChanged
        );

        messageSenders[sender->id] = sender;
        messageSenderConfigurations[sender->id] = configuration;
        messageSenderStatuses[sender->id] = {};
    }
}

void
TcpMessageSendersManager::handleMessageSenderStatusChanged(MessageSenderStatusChanged n) {
    messageSenderStatuses[n.senderId].state = n.lastState;
    messageSenderStatuses[n.senderId].error = n.lastError;
}
