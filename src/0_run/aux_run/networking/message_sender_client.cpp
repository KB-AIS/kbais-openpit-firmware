#include "message_sender_client.h"

// plog
#include <plog/Log.h>

using namespace kbais::cfw::networking;

const auto SocketErrorSignal = qOverload<SocketError>(&QAbstractSocket::error);

const auto SocketStateSignal = qOverload<SocketState>(&QAbstractSocket::stateChanged);

MessageSenderClient::MessageSenderClient(QObject *parent) : QObject(parent) {
    connect(&sendMessagesTimer, &QTimer::timeout, this, [&] {
        const auto batches = getMessageBatchesQueryHandler.query(5);

        if (batches.isEmpty()) {
            return ;
        }

        socket.write(batches.first().messages.first().payload);
    });

    connect(&socket, &QTcpSocket::readyRead, this, [&] {
        PLOGD << socket.readAll();
    });

    connect(&socket, &QTcpSocket::connected, this, [&] {
        PLOGD << "connected";
        sendMessagesTimer.start();
    });

    connect(&socket, SocketErrorSignal, this, [&](auto error) {
        PLOGD << "error: " << QVariant::fromValue(error).toString();
        sendMessagesTimer.stop();
    });

    connect(&socket, SocketStateSignal, this, [&](auto state) {
        PLOGD << "state: " << QVariant::fromValue(state).toString();
        emit notifyStateChanged(state);
    });

    connect(&socket, &QTcpSocket::disconnected, this, [&] {
        PLOGD << "disconnected";
    });
}

void
MessageSenderClient::restart(const MessageSenderParams& params) {
    sendMessagesTimer.stop();

    sendMessagesTimer.setInterval(params.requestInterval);

    socket.connectToHost(params.host, params.port);
}
