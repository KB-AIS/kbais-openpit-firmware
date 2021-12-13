#include "message_sender_client.h"

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

MessageSender::MessageSender(
    MessageSenderConfiguration configuration
) : QObject(), _configuration { configuration } {
    QObject::connect(&_reducer, &QStateMachine::started, this, [&] {
        PLOGD << "reducer started";

        emit eCONNECT_REQUESTED();
    });

    // ON IDLE
    sIDLE
        .addTransition(this, &MessageSender::eCONNECT_REQUESTED, &sCONNECTING);

    QObject::connect(&sIDLE, &QState::exited, this, [&] {
        PLOGD << "IDLE exited";

        _socket.connectToHost(_configuration.host, _configuration.port);
        emit eCONNECT_REQUESTED();
    });

    // ON CONNECTING
    sCONNECTING
        .addTransition(&_socket, &QTcpSocket::connected, &sCONNECTED);

    QObject::connect(&sCONNECTING, &QState::entered, this, [&] {
        PLOGD << "CONNECTING entered";
    });

    // ON CONNECTED
    QObject::connect(&sCONNECTED, &QState::entered, this, [&] {
        PLOGD << "CONNECTED entered";

        _timerSendReqeust.start();
    });

    // SOCKET
    QObject::connect(&_socket, &QTcpSocket::connected, this, [&] {

    });

    QObject::connect(&_socket, SocketStateSignal, this, [&](auto state) {
        emit notifyStateChanged(id, state);
    });

    QObject::connect(&_socket, SocketErrorSignal, this, [&](auto error) {
        PLOGD << "error: " << QVariant::fromValue(error).toString();
        _timerSendReqeust.stop();
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

    // TIMER
    _timerSendReqeust.setInterval(_configuration.requestInterval);

    QObject::connect(&_timerSendReqeust, &QTimer::timeout, this, [&] {
        const auto batches = queryHandler.query(1);

        if (batches.isEmpty()) {
            return;
        }

        _socket.write(batches.first().messages.first().payload);
    });

    _reducer.addState(&sIDLE);
    _reducer.addState(&sCONNECTING);
    _reducer.addState(&sCONNECTED);
    _reducer.setInitialState(&sIDLE);
}

void
MessageSender::restart() {
    _timerSendReqeust.stop();

    _reducer.stop();

    _reducer.start();
}
