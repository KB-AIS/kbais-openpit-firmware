#include "swom_protocol_communicator.h"

// qt
#include <QUuid>
#include <QDataStream>
// oss
#include <nlohmann/json.hpp>
#include <QThread>
#include <plog/Log.h>

using namespace std::chrono_literals;

using namespace KbAis::Cfw::Networking;

using json = nlohmann::json;

constexpr std::chrono::milliseconds REPLAY_TIMEOUT { 30s };

SwomProtocolCommunicator::SwomProtocolCommunicator() {
    timerWaitReplay.setInterval(REPLAY_TIMEOUT);
}

void
SwomProtocolCommunicator::start(QIODevice& device) {
    // Read incomming data from server side.
    connectionReadData = QObject::connect(&device, &QIODevice::readyRead, [&] {
        auto state = currentState();

        if (state == SwomProtocolCommunicatorState::Authenticating) {
            timerWaitReplay.stop();

            // TODO: Process result with formatter
            auto uuid = QUuid::fromRfc4122(device.read(1024));
            PLOGD << "Recived uuid: " << uuid.toString();

            this->state = SwomProtocolCommunicatorState::ReadyToSendData;
            return timerSendData.start(10s);
        }

        if (state != SwomProtocolCommunicatorState::WaitReplayOnDataSent) {
            // TODO: Process unexpected data read
            return;
        }
    });

    auto sendDataHandler = [&] {
        auto state = currentState();

        if (state != SwomProtocolCommunicatorState::ReadyToSendData) {
            return;
        }

        // TODO: Process result with formatter
        const auto batches = queryHandler.query(3);
        if (batches.isEmpty()) return;

        QByteArray bytes;
        QDataStream bStream(&bytes, QIODevice::WriteOnly);

        bStream << static_cast<quint8>(DataType::Tel);
        for (const auto& batch: batches) {
            for (const auto& message: batch.messages) {
                auto monkierBytes = json::to_msgpack(message.moniker.toStdString());

                bStream << QByteArray(reinterpret_cast<const char*>(monkierBytes.data()), monkierBytes.size());
                bStream << message.payload;
            }
        }
        device.write(bytes);

        timerWaitReplay.stop();
    };

    connectionSendData = QObject::connect(&timerSendData, &QTimer::timeout, sendDataHandler);

    QObject::connect(
        this, &SwomProtocolCommunicator::notifyNeedSend,

        this, sendDataHandler, Qt::BlockingQueuedConnection
    );

    // Process
    connectionWaitReplay = QObject::connect(&timerWaitReplay, &QTimer::timeout, [&] {
        PLOGD << "Waiting for replay timed out";

        device.close();
    });

    requestAuthentication(device);
}

void
SwomProtocolCommunicator::cease() {
    timerWaitReplay.stop();

    timerSendData.stop();

    QObject::disconnect(connectionReadData);

    QObject::disconnect(connectionSendData);

    QObject::disconnect(connectionWaitReplay);
}

void
SwomProtocolCommunicator::sendMessage() {
    emit notifyNeedSend();
}

SwomProtocolCommunicatorState
SwomProtocolCommunicator::currentState() const { return state; }

void
SwomProtocolCommunicator::requestAuthentication(QIODevice &device) {
    device.write(formatter.encodeAuthPackage("104"));

    timerWaitReplay.start();

    state = SwomProtocolCommunicatorState::Authenticating;
}
