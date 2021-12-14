#include "message_sender.h"

// plog
#include <plog/Log.h>

using namespace kbais::cfw::networking;

const auto SocketErrorSignal = qOverload<SocketError>(&QAbstractSocket::error);

const auto SocketStateSignal = qOverload<SocketState>(&QAbstractSocket::stateChanged);

namespace kbais::cfw::networking {

void to_json(json& j, const Ack& x) {
    j = json {
        { "status", x.status.toStdString() },
    };
}

void from_json(const json& j, Ack& x) {
    std::string statusStr {};

    j.at("status").get_to(statusStr);

    x.status = QString::fromStdString(statusStr);
}

}

MessageSender::MessageSender() : QObject() {
    QObject::connect(&_reducer, &QStateMachine::started, this, [&] {
        QObject::disconnect(_connectionReducerRestart);

        emit eCONNECT_REQUESTED();
    });

    // ON IDLE
    stateIdle
        .addTransition(this, &MessageSender::eCONNECT_REQUESTED, &sCONNECTING);

    QObject::connect(&stateIdle, &QState::exited, this, [&] {
        PLOGD << "IDLE exited";

        _socket.connectToHost(host, port);
        emit eCONNECT_REQUESTED();
    });

    _reducer.addState(&stateIdle);

    // ON CONNECTING
    sCONNECTING
        .addTransition(&_socket, &QTcpSocket::connected, &sCONNECTED);

    QObject::connect(&sCONNECTING, &QState::entered, this, [&] {
        PLOGD << "CONNECTING entered";
    });

    _reducer.addState(&sCONNECTING);

    // ON CONNECTED
    QObject::connect(&sCONNECTED, &QState::entered, this, [&] {
        PLOGD << "CONNECTED entered";

        _timerSendReqeust.start();
    });

    _reducer.addState(&sCONNECTED);

    // Configure 'SOCKET' events
    QObject::connect(&_socket, &QTcpSocket::connected, this, [&] {

    });

    QObject::connect(&_socket, SocketStateSignal, this, [&](auto state) {
        emit notifyStatusChanged(id, state, _socket.error());
    });

    QObject::connect(&_socket, SocketErrorSignal, this, [&](auto error) {
        _timerSendReqeust.stop();

        emit notifyStatusChanged(id, _socket.state(), error);
    });

    QObject::connect(&_socket, &QTcpSocket::readyRead, this, [&] {
        auto bytes = _socket.read(128);

        Ack answer = json::from_msgpack(
            QByteArray(reinterpret_cast<const char*>(bytes.data()), bytes.size())
        );

        PLOGD << "ACK: " << answer.status;
    });

    QObject::connect(&_socket, &QTcpSocket::disconnected, this, [&] {

    });

    QObject::connect(&_timerSendReqeust, &QTimer::timeout, this, [&] {
        const auto batches = queryHandler.query(1);

        if (batches.isEmpty()) {
            return;
        }

        _socket.write(batches.first().messages.first().payload);
    });

    _reducer.setInitialState(&stateIdle);
}

void
MessageSender::restart(MessageSenderConfiguration configuration) {
    _timerSendReqeust.stop();

    _timerSendReqeust.setInterval(configuration.requestInterval);

    host = configuration.host;

    port = configuration.port;

    // Restart reducer after full stop
    restartReducer();
}

void
MessageSender::restartReducer() {
    if (!_reducer.isRunning()) {
        return _reducer.start();
    }

    _connectionReducerRestart = QObject::connect(
        &_reducer, &QStateMachine::stopped, this, [&] { _reducer.start(); }
    );

    _reducer.stop();
}
