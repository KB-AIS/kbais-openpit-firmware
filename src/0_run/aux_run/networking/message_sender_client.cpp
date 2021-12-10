#include "message_sender_client.h"

using namespace kbais::cfw::networking;

const auto SocketErrorSignal = qOverload<SocketError>(&QAbstractSocket::error);

const auto SocketStateSignal = qOverload<SocketState>(&QAbstractSocket::stateChanged);

MessageSenderClient::MessageSenderClient(QObject *parent) : QObject(parent) {
    connect(&sendMessagesTimer, &QTimer::timeout, this, [&] {
        const auto batches = getMessageBatchesQueryHandler.query(5);

        if (batches.isEmpty()) {
            return ;
        }
        // TODO: Format message
        socket.write(batches.first().messages.first().payload);
    });

    // TODO: Use QStateMachine to switch state whether or no device is auth
    connect(&socket, &QTcpSocket::readyRead, this, [&] {
        // Check ACK
    });

    connect(&socket, &QTcpSocket::connected, this, [&] {
        sendMessagesTimer.start();
    });

    connect(&socket, SocketErrorSignal, this, [&](auto error) {
        sendMessagesTimer.stop();
    });

    connect(&socket, SocketStateSignal, this, [&](auto state) {

    });

    connect(&socket, &QTcpSocket::disconnected, this, [&] {

    });
}

void
MessageSenderClient::restart(const MessageSenderParams& params) {
    sendMessagesTimer.stop();

    sendMessagesTimer.setInterval(params.requestInterval);

    socket.connectToHost(params.host, params.port);
}
