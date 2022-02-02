#include "TcpMessageSender.h"

// oss
#include <plog/Log.h>
#include <QMetaEnum>

template<typename QEnum>
std::string QtEnumToString(const QEnum value) {
    return std::string(QMetaEnum::fromType<QEnum>().valueToKey(value));
}

TcpMessageSender::TcpMessageSender()
    :   QObject()
    ,   m_socket(this)
{
    setup_socket_signals();
}

void
TcpMessageSender::setup_socket_signals() {
    constexpr auto SocketStateSignal = qOverload<SocketState>(&QAbstractSocket::stateChanged);

    constexpr auto SocketErrorSignal = qOverload<SocketError>(&QAbstractSocket::error);

    QObject::connect(&m_socket, &QTcpSocket::connected, [&] {
        PLOGD << "Connected";
    });

    QObject::connect(&m_socket, SocketStateSignal, [&](auto state) {
        PLOGD << "State " << QtEnumToString(state);
    });

    QObject::connect(&m_socket, SocketErrorSignal, [&](auto error) {
        PLOGD << "Error " << QtEnumToString(error);
    });

    QObject::connect(&m_socket, &QTcpSocket::disconnected, [&] {
        PLOGD << "Disconnected";
    });
}

void
TcpMessageSender::restart(const MessageSenderConfiguration& configuration) {
    m_socket.connectToHost(configuration.host, configuration.port);
}
