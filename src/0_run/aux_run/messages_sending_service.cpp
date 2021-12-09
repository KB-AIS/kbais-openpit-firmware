#include "messages_sending_service.h"

// plog
#include <plog/Log.h>
// Qt
#include <QtCore/QtGlobal>

MessagesSendingService::MessagesSendingService(

    QObject *parent
) : QObject(parent) {
    QObject::connect(&_socket, &QTcpSocket::connected, this, [&] {
        PLOGD << "Connected";

    });

    QObject::connect(&_socket, qOverload<SktError>(&QAbstractSocket::error), this, [&](auto e) {
        PLOGD << "Socket return error: " << QVariant::fromValue(e).toString();
    });

    QObject::connect(&_socket, &QTcpSocket::disconnected, this, [&] {
        PLOGD << "Disconnected";

        // TODO: Breake sending loop and request reconnection
    });


    PLOGD << "Starting connect to server";
    _socket.connectToHost("192.168.0.104", 9900);
    _socket.waitForConnected();
}

void MessagesSendingService::sendMessages(QList<DeviceMessage> messages) {
    // TODO: Pass messages to queue
    PLOGD << "Socket status: " << QVariant::fromValue(_socket.state()).toString();

    if (_socket.state() != SktState::ConnectedState) {
        return;
    }

    const auto bytesSent = _socket.write(messages.first().payload);
    _socket.flush();

    PLOGD << "Bytes sent: " << bytesSent;
}

void MessagesSendingService::handleSocketError(SktError error) {

}
