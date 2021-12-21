#include "message_sender.h"

static const auto SocketStateSignal = qOverload<SocketState>(&QAbstractSocket::stateChanged);

static const auto SocketErrorSignal = qOverload<SocketError>(&QAbstractSocket::error);

MessageSender::MessageSender() : QObject() {
    connectSocketSignals();
}

void
MessageSender::connectSocketSignals() {
    QObject::connect(&socket, &QTcpSocket::connected, [&] {
        communicator->start(socket);
    });

    QObject::connect(&socket, SocketStateSignal, [&](auto state) {
        emit notifyStatusChanged({ id, state, socket.error() });
    });

    QObject::connect(&socket, SocketErrorSignal, [&](auto error) {
        emit notifyStatusChanged({ id, socket.state(), error });
    });

    QObject::connect(&socket, &QTcpSocket::disconnected, [&] {
        communicator->cease();
    });
}

void
MessageSender::sendMessage() {
    if (communicator) {
        communicator->sendMessage();
    }
}

void
MessageSender::restart(const MessageSenderConfiguration& configuration) {
    // Ask a communicator to stop if restart has been called before socket
    // disconnected from a host.
    if (communicator) {
        communicator->cease();
    }

    communicator = configuration.communicator;

    socket.connectToHost(configuration.host, configuration.port);
}
