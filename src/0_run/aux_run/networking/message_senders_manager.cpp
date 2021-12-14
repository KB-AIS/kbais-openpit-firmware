#include "message_senders_manager.h"

// std
#include <chrono>
// qt
#include <QVariant>
// oss
#include <plog/Log.h>

using namespace std::chrono_literals;

using namespace kbais::cfw::networking;

constexpr std::chrono::milliseconds RESTART_MESSAGE_SENDERS_INTERVAL { 5s };

MessageSendersManager::MessageSendersManager(
    QObject* parent
) : QObject(parent) {
    _restartMessageSendersTimer.setInterval(RESTART_MESSAGE_SENDERS_INTERVAL);

    QObject::connect(
        &_restartMessageSendersTimer, &QTimer::timeout,
        this, &MessageSendersManager::restartMessageSenders
    );

    _restartMessageSendersTimer.start();
}

void
MessageSendersManager::handleConfigurationChanged(
    const QList<MessageSenderConfiguration>& configurations
) {
    for (const auto& configuration : configurations) {
        const auto sender = new MessageSender();

        QObject::connect(
            sender, &MessageSender::notifyStatusChanged,

            this, &MessageSendersManager::handleMessageSenderStatusChanged
        );

        _messageSenders[sender->id] = sender;
        _messageSenderConfigurations[sender->id] = configuration;
        _messageSenderStatuses[sender->id] = {};
    }
}

void
MessageSendersManager::restartMessageSenders() {
    for (const auto senderId : _messageSenderStatuses.keys()) {
        const auto status = _messageSenderStatuses[senderId];

        const auto hasSenderValidStatus =
            status.lastState != SocketState::UnconnectedState;

        if (hasSenderValidStatus) continue;

        const auto configuration = _messageSenderConfigurations[senderId];
        _messageSenders[senderId]->restart(configuration);
    }
}

void
MessageSendersManager::handleMessageSenderStatusChanged(
    QUuid senderId, SocketState lastState, SocketError lastError
) {
    _messageSenderStatuses[senderId].lastState = lastState;
    _messageSenderStatuses[senderId].lastError = lastError;
}
