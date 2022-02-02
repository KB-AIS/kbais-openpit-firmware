#include "TcpMessageSender.h"

TcpMessageSender::TcpMessageSender()
    :   QObject()
    ,   mSocket(this)
{
    setupSocketSignals();
}

void
TcpMessageSender::setupSocketSignals() {
    constexpr auto SocketStateSignal = qOverload<SocketState>(&QAbstractSocket::stateChanged);

    constexpr auto SocketErrorSignal = qOverload<SocketError>(&QAbstractSocket::error);

    QObject::connect(&mSocket, &QTcpSocket::connected, [&] {

    });

    QObject::connect(&mSocket, SocketStateSignal, [&](auto state) {

    });

    QObject::connect(&mSocket, SocketErrorSignal, [&](auto error) {

    });

    QObject::connect(&mSocket, &QTcpSocket::disconnected, [&] {

    });
}

void
TcpMessageSender::restart(const MessageSenderConfiguration& configuration) {
    mSocket.connectToHost(configuration.host, configuration.port);
}
