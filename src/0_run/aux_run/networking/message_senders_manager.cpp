#include "message_senders_manager.h"

// qt
#include <QVariant>
// oss
#include <plog/Log.h>

using namespace kbais::cfw::networking;

MessageSendersManager::MessageSendersManager(QObject* parent) : QObject(parent) {
    _senderConfigurations.push_back({
        "10.214.1.247",
        9900,
        std::chrono::milliseconds { 10000 },
        BaseProtocolFormatter { }
    });

    _senderConfigurations.push_back({
        "10.214.1.208",
        9900,
        std::chrono::milliseconds { 5000 },
        BaseProtocolFormatter { }
    });
}

void MessageSendersManager::handleConfiguratingChanged() {
    for (auto configuration : _senderConfigurations) {
        auto sender = new MessageSender(configuration);

        QObject::connect(
            sender, &MessageSender::notifyStateChanged,

            this, &MessageSendersManager::handleSenderStateChanged
        );

        _senders[sender->id] = sender;

        sender->restart();
    }
}

void MessageSendersManager::handleSenderStateChanged(QUuid id, SocketState state) {
    // TODO: Schedule restart
}
