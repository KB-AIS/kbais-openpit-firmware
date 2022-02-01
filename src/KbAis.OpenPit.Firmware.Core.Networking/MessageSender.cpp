#include "MessageSender.h"

static const auto SocketStateSignal = qOverload<SocketState>(&QAbstractSocket::stateChanged);

static const auto SocketErrorSignal = qOverload<SocketError>(&QAbstractSocket::error);

MessageSender::MessageSender(QSharedPointer<IProtocolCommunicator> communicator) : QObject(),
    socket(this),
    communicator { communicator } {
    connectSocketSignals();
}

void
MessageSender::connectSocketSignals() {
    QObject::connect(&socket, &QTcpSocket::connected, [&] {
        communicator->beginCommunication(socket);
    });

    QObject::connect(&socket, SocketStateSignal, [&](auto state) {
        emit notifyStatusChanged({ id, state, socket.error() });
    });

    QObject::connect(&socket, SocketErrorSignal, [&](auto error) {
        emit notifyStatusChanged({ id, socket.state(), error });
    });

    QObject::connect(&socket, &QTcpSocket::disconnected, [&] {
        communicator->endCommunication();
    });
}

void
MessageSender::sendMessage() {
    if (communicator) {
        communicator->sendNow();
    }
}

void
MessageSender::restart(const MessageSenderConfiguration& configuration) {
    // Ask a communicator to stop if restart has been called before socket
    // disconnected from a host.
    communicator->endCommunication();

    socket.connectToHost(configuration.host, configuration.port);
}