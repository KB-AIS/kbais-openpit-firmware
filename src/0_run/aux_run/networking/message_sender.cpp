#include "message_sender.h"

// oss
#include <plog/Log.h>

using namespace KbAis::Cfw::Networking;

MessageSender::MessageSender() : QObject() {
    QObject::connect(&reducer, &QStateMachine::started, this, [&] {
        QObject::disconnect(cReducerStopped);

        emit notifyConnectRequested();
    });

    sIdle.addTransition(this, &MessageSender::notifyConnectRequested, &sConnecting);

    sConnecting.addTransition(&socket, &QTcpSocket::connected, &sConnected);

    reducer.addState(&sIdle);

    reducer.addState(&sConnecting);

    reducer.addState(&sConnected);

    reducer.setInitialState(&sIdle);

    QObject::connect(&sIdle, &QState::exited, [&] {
        socket.connectToHost(host, port);
        emit notifyConnectRequested();
    });

    QObject::connect(&sConnected, &QState::entered,[&] {
        communicator->startCommunication(socket);
    });

    connectSocketSignals();
}

void
MessageSender::connectSocketSignals() {
    const auto SocketStateSignal = qOverload<SocketState>(&QAbstractSocket::stateChanged);
    QObject::connect(&socket, SocketStateSignal, [&](auto state) {
        emit notifyStatusChanged({ id, state, socket.error() });
    });

    const auto SocketErrorSignal = qOverload<SocketError>(&QAbstractSocket::error);
    QObject::connect(&socket, SocketErrorSignal, [&](auto error) {
        emit notifyStatusChanged({ id, socket.state(), error });
    });

    QObject::connect(&socket, &QTcpSocket::disconnected, [&] {
        communicator->ceaseCommunication();
    });
}

void
MessageSender::restart(const MessageSenderConfiguration& configuration) {
    host = configuration.host;
    port = configuration.port;

    // Ask a communicator to stop if restart has been called before socket
    // disconnected from a host.
    if (communicator) {
        communicator->ceaseCommunication();
    }

    communicator = configuration.communicator;

    // Try to gracefully restart running reducer.
    if (!reducer.isRunning()) {
        return reducer.start();
    }

    cReducerStopped =
        QObject::connect(&reducer, &QStateMachine::stopped, [&] { reducer.start(); });

    reducer.stop();
}
