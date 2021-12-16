#include "swom_protocol_communicator.h"

// qt
#include <QUuid>
// oss
#include <plog/Log.h>

using namespace KbAis::Cfw::Networking;

void
SwomProtocolCommunicator::startCommunication(QIODevice& device) {
    auto authPackageData = formatter.encodeAuthPackage("104");

    device.write(authPackageData);

    cReadyRead = QObject::connect(&device, &QIODevice::readyRead, [&] {
        auto data = device.peek(1024);

        auto uuid = QUuid::fromRfc4122(data);

        PLOGD << "Incomming uuid: " << uuid.toString();

        device.read(1024);
    });
}

void
SwomProtocolCommunicator::ceaseCommunication() {
    QObject::disconnect(cReadyRead);
}


