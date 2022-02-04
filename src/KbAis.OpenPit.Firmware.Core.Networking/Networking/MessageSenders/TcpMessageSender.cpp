#include "TcpMessageSender.h"

// qt
#include <QMetaEnum>
// oss
#include <plog/Log.h>

#include "Networking/Communicators/Swom/SwomProtocolCommunicator.h"

template<typename TEnum>
QString QEnumToString(const TEnum value) {
    return QMetaEnum::fromType<TEnum>().valueToKey(value);
}

TcpMessageSender::TcpMessageSender(const QString& message_sender_name)
    :   QObject()
    ,   m_messageSenderName(message_sender_name)
    ,   m_socket(this)
{
    SetupSocketSignals();
}

void
TcpMessageSender::Restart(const TcpMessageSenderConfiguration& configuration) {
    if (m_protocolCommunicator != nullptr) {
        // TODO: Should we just use destructor to stop communication?
        m_protocolCommunicator->StopCommunication();
    }

    m_protocolCommunicator = GenProtocolCommunicator(configuration.protocol);

    // Connecto to new host
    m_socket.connectToHost(configuration.host, configuration.port);
}

void
TcpMessageSender::SetupSocketSignals() {
    using SocketState = QAbstractSocket::SocketState;
    constexpr auto SocketStateSignal = qOverload<SocketState>(&QAbstractSocket::stateChanged);

    using SocketError = QAbstractSocket::SocketError;
    constexpr auto SocketErrorSignal = qOverload<SocketError>(&QAbstractSocket::error);

    QObject::connect(&m_socket, &QTcpSocket::connected, [&] {
        PLOGD << m_messageSenderName << " -- connected";

        m_protocolCommunicator->InitCommunication(m_socket);
    });

    QObject::connect(&m_socket, SocketStateSignal, [&](auto state) {
        PLOGD << m_messageSenderName << " -- change state: " << QEnumToString(state);

        emit StateChanged({ m_messageSenderName, { state, m_socket.error() } });
    });

    QObject::connect(&m_socket, SocketErrorSignal, [&](auto error) {
        PLOGE << m_messageSenderName << " -- got error: " << QEnumToString(error);

        emit StateChanged({ m_messageSenderName, { m_socket.state(), error } });
    });

    QObject::connect(&m_socket, &QTcpSocket::disconnected, [&] {
        PLOGD << m_messageSenderName << " -- disconnected";

        if (m_protocolCommunicator != nullptr) {
            m_protocolCommunicator->StopCommunication();
        }
    });
}

TcpMessageSender::ProtocolCommunicator_t
TcpMessageSender::GenProtocolCommunicator(const MessageSenderProtocol protocol) {
    switch (protocol) {

    case MessageSenderProtocol::Swom:
        return std::make_unique<SwomProtocolCommunicator>();

    default: throw std::logic_error { "Unknown protocol" };

    }
}
