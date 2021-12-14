#include "message_senders_manager.h"

// std
#include <chrono>
// qt
#include <QVariant>
// oss
#include <plog/Log.h>

using namespace std::chrono_literals;

using namespace kbais::cfw::networking;

constexpr std::chrono::milliseconds RESTART_MESSAGESENDERS_INTERVAL { 5s };

MessageSendersManager::MessageSendersManager(
    QObject* parent
) : QObject(parent) {
    _restartMessageSendersTimer.setInterval(RESTART_MESSAGESENDERS_INTERVAL);

    QObject::connect(
        &_restartMessageSendersTimer, &QTimer::timeout,

        this, &MessageSendersManager::restartMessageSenders
    );

    _restartMessageSendersTimer.start();
}

void
MessageSendersManager::handleConfiguratingChanged(
    const QList<MessageSenderConfiguration>& configurations
) {
    for (const auto& configuration : configurations) {
        auto sender = new MessageSender();

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
    for (auto senderId : _messageSenderStatuses.keys()) {
        auto status = _messageSenderStatuses[senderId];

        auto hasSenderValidStatus = status.lastState != SocketState::UnconnectedState;

        if (hasSenderValidStatus) continue;

        _messageSenders[senderId]->restart(_messageSenderConfigurations[senderId]);
    }
}

void
MessageSendersManager::handleMessageSenderStatusChanged(
    QUuid id, SocketState lastState, SocketError lastError
) {
    _messageSenderStatuses[id].lastState = lastState;
    _messageSenderStatuses[id].lastError = lastError;
}
