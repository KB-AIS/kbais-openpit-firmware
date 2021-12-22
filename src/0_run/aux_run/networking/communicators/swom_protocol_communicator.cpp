#include "swom_protocol_communicator.h"

// qt
#include <QUuid>
#include <QDataStream>
// oss
#include <nlohmann/json.hpp>
#include <QThread>

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds REPLAY_TIMEOUT { 30s };

SwomProtocolCommunicator::SwomProtocolCommunicator() {
    timerWaitAcknowledge.setInterval(REPLAY_TIMEOUT);
}

void
SwomProtocolCommunicator::beginCommunication(QIODevice& device) {
    // Read incomming data from server side.
    connectionReadData = QObject::connect(&device, &QIODevice::readyRead, [&] {
        auto state = getCurrentState();

        if (state == SwomProtocolCommunicatorState::Authenticating) {
            timerWaitAcknowledge.stop();

            // TODO: Process result with formatter
            auto uuid = QUuid::fromRfc4122(device.read(1024));

            this->state = SwomProtocolCommunicatorState::ReadyToSendData;
            return timerSendMessagesBatches.start(10s);
        }

        if (state != SwomProtocolCommunicatorState::WaitReplayOnDataSent) {

            return;
        }
    });

    auto sendMessagesBatchesHandler = [&] {
        auto state = getCurrentState();

        if (state != SwomProtocolCommunicatorState::ReadyToSendData) {
            return;
        }

        const auto batches = getMessagesBatchesQuery.handle(10);
        if (batches.isEmpty()) {
            return;
        }

        device.write(formatter.encodeTelFrame(batches));
    };

    QObject::connect(
        &timerSendMessagesBatches, &QTimer::timeout,

        this, sendMessagesBatchesHandler, Qt::BlockingQueuedConnection
    );

    QObject::connect(
        this, &SwomProtocolCommunicator::notifyNeedSend,

        this, sendMessagesBatchesHandler, Qt::BlockingQueuedConnection
    );

    connectionWaitReplay = QObject::connect(&timerWaitAcknowledge, &QTimer::timeout, [&] {
        // PLOGD << "Waiting for replay timed out";

        device.close();
    });

    requestAuthentication(device);
}

void
SwomProtocolCommunicator::interruptCommunication() {
    timerWaitAcknowledge.stop();

    timerSendMessagesBatches.stop();

    QObject::disconnect(connectionReadData);

    QObject::disconnect(connectionSendData);

    QObject::disconnect(connectionWaitReplay);
}

void
SwomProtocolCommunicator::sendMessage() {
    emit notifyNeedSend();
}

SwomProtocolCommunicatorState
SwomProtocolCommunicator::getCurrentState() const { return state; }

void
SwomProtocolCommunicator::requestAuthentication(QIODevice &device) {
    device.write(formatter.encodeAthFrame("104"));

    timerWaitAcknowledge.start();

    state = SwomProtocolCommunicatorState::Authenticating;
}
