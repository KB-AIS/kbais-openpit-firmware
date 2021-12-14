#include "message_sender.h"

// oss
#include <plog/Log.h>

using namespace kbais::cfw::networking;

MessageSender::MessageSender() : QObject() {
    QObject::connect(&_reducer, &QStateMachine::started, this, [&] {
        QObject::disconnect(_cReducerRestart);

        emit eventConnectRequested();
    });

    stateIdle
        .addTransition(this, &MessageSender::eventConnectRequested, &stateConnecting);

    _reducer.addState(&stateIdle);

    // Request connect to provided host on exiting from 'IDLE' state. This
    // will triger transition to 'CONNECTING' state.
    QObject::connect(&stateIdle, &QState::exited, this, [&] {
        _socket.connectToHost(host, port);
        emit eventConnectRequested();
    });

    stateConnecting
        .addTransition(&_socket, &QTcpSocket::connected, &stateConnected);

    _reducer.addState(&stateConnecting);

//    stateConnected
//        .addTransition(this, &MessageSender::eventConnectRequested);

    _reducer.addState(&stateConnected);

    QObject::connect(&stateConnected, &QState::entered, this, [&] {
        // TODO: Request auth

        // _timerSendReqeust.start();
    });

    connectSocketSignals();

    QObject::connect(&_timerSendMessages, &QTimer::timeout, this, [&] {
        // TODO: Impl datagram request
    });

    _reducer.setInitialState(&stateIdle);
}

void
MessageSender::connectSocketSignals() {
    const auto SocketStateSignal = qOverload<SocketState>(&QAbstractSocket::stateChanged);
    QObject::connect(&_socket, SocketStateSignal, this, [&](auto state) {
        emit notifyStatusChanged(id, state, _socket.error());
    });

    const auto SocketErrorSignal = qOverload<SocketError>(&QAbstractSocket::error);
    QObject::connect(&_socket, SocketErrorSignal, this, [&](auto error) {
        emit notifyStatusChanged(id, _socket.state(), error);
    });

    // Stop messages sending loop on TCP socket disconnect.
    QObject::connect(&_socket, &QTcpSocket::disconnected, this, [&] {
        _timerSendMessages.stop();
    });
}

void
MessageSender::restart(MessageSenderConfiguration configuration) {
    const auto restartReducer = [&] {
        if (!_reducer.isRunning()) {
            return _reducer.start();
        }

        _cReducerRestart = QObject::connect(&_reducer, &QStateMachine::stopped, this, [&] {
            _reducer.start();
        });

        _reducer.stop();
    };

    _timerSendMessages.stop();

    _timerSendMessages.setInterval(configuration.intervalSendMessages);

    host = configuration.host;

    port = configuration.port;

    restartReducer();
}
