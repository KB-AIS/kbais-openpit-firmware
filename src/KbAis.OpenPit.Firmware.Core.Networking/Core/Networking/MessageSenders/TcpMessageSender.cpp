#include "TcpMessageSender.h"

// qt
#include <QMetaEnum>
// oss
#include <plog/Log.h>

#include "Core/Networking/Communicators/Swom/SwomProtocolCommunicator.h"
#include "Format.h"

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
TcpMessageSender::Restart(const TcpMessageSenderConfiguration& configuration, const QString& equipment_id) {
    if (m_protocolCommunicator != nullptr) {
        m_protocolCommunicator->StopCommunication();

        m_subCommunicator.unsubscribe();
        m_subCommunicator = rxcpp::composite_subscription();
    }

    m_protocolCommunicator = GenProtocolCommunicator(configuration.protocol);
    m_equipment_id = equipment_id;

    m_protocolCommunicator->GetObservableProtocolViolation()
        .subscribe(m_subCommunicator, [&](const ProtocolViolationNotif& x) {
            PLOGW << fmt::format("Communicator got prtocol violation: {}", x.message);
            m_socket.disconnectFromHost();
        });

    m_socket.connectToHost(configuration.host, configuration.port);
}

void
TcpMessageSender::SetupSocketSignals() {
    using SocketState = QAbstractSocket::SocketState;
    constexpr auto SocketStateSignal = qOverload<SocketState>(&QAbstractSocket::stateChanged);

    using SocketError = QAbstractSocket::SocketError;
    constexpr auto SocketErrorSignal = qOverload<SocketError>(&QAbstractSocket::error);

    QObject::connect(&m_socket, &QTcpSocket::connected, [&] {
        m_protocolCommunicator->InitCommunication(m_socket, m_equipment_id);
    });

    QObject::connect(&m_socket, SocketStateSignal, [&](auto state) {
        emit StateChanged({ m_messageSenderName, { state, m_socket.error() } });
    });

    QObject::connect(&m_socket, SocketErrorSignal, [&](auto error) {
        emit StateChanged({ m_messageSenderName, { m_socket.state(), error } });
    });

    QObject::connect(&m_socket, &QTcpSocket::disconnected, [&] {
        if (m_protocolCommunicator != nullptr) {
            m_protocolCommunicator->StopCommunication();
        }
    });
}

// TODO: To abstract factory with boost::di
TcpMessageSender::ProtocolCommunicator_t
TcpMessageSender::GenProtocolCommunicator(const MessageSenderProtocol protocol) {
    switch (protocol) {

    case MessageSenderProtocol::Swom:
        return std::make_unique<SwomProtocolCommunicator>();

    default: throw std::logic_error { "Unknown protocol" };

    }
}
