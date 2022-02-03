#include "TcpMessageSender.h"

// oss
#include <plog/Log.h>
#include <QMetaEnum>

template<typename QEnum>
std::string qt_enum_to_string(const QEnum value) {
    return std::string(QMetaEnum::fromType<QEnum>().valueToKey(value));
}

TcpMessageSender::TcpMessageSender(const QString& message_sender_name)
    :   QObject()
    ,   m_message_sender_name(message_sender_name)
    ,   m_socket(this)
{
    setup_socket_signals();
}

void
TcpMessageSender::setup_socket_signals() {
    constexpr auto SocketStateSignal = qOverload<SocketState>(&QAbstractSocket::stateChanged);

    constexpr auto SocketErrorSignal = qOverload<SocketError>(&QAbstractSocket::error);

    QObject::connect(&m_socket, &QTcpSocket::connected, [&] {
        PLOGD << m_message_sender_name << " -- connected";
    });

    QObject::connect(&m_socket, SocketStateSignal, [&](auto state) {
        PLOGD << m_message_sender_name << " -- change state: " << qt_enum_to_string(state);
    });

    QObject::connect(&m_socket, SocketErrorSignal, [&](auto error) {
        PLOGE << m_message_sender_name << " -- got error: " << qt_enum_to_string(error);
    });

    QObject::connect(&m_socket, &QTcpSocket::disconnected, [&] {
        PLOGD << m_message_sender_name << " -- disconnected";
    });
}

void
TcpMessageSender::restart(const MessageSenderConfiguration& configuration) {
    m_socket.connectToHost(configuration.host, configuration.port);
}
