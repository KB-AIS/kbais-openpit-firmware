#include "swom_protocol_communicator.h"

// qt
#include <QDataStream>
#include <QThread>
#include <QUuid>
#include <QDebug>
// oss
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds REPLAY_TIMEOUT { 30s };

SwomProtocolCommunicator::SwomProtocolCommunicator() :
    tWaitAcknowledge { this },
    tSendDataReccurently { this } {
    tWaitAcknowledge.setInterval(REPLAY_TIMEOUT);
}

void
SwomProtocolCommunicator::beginCommunication(QIODevice& device) {
    // Read incomming data from server side.
    cReadData = QObject::connect(&device, &QIODevice::readyRead, [&] {
        tWaitAcknowledge.stop();

        auto state = getCurrentState();

        if (state == SwomProtocolCommunicatorState::Authenticating) {
            // TODO: Process result with formatter
            auto uuid = QUuid::fromRfc4122(device.read(1024));

            this->state = SwomProtocolCommunicatorState::ReadyToSendData;
            return tSendDataReccurently.start(10s);
        }

        if (state == SwomProtocolCommunicatorState::WaitAcknowledge) {
            // TODO: Decode ACK
            auto uuid = QUuid::fromRfc4122(device.read(1024));
            qDebug() << uuid;

            this->state = SwomProtocolCommunicatorState::ReadyToSendData;
            return tSendDataReccurently.start(10s);
        }
    });

    auto sendDataHandler = [&] {
        tSendDataReccurently.stop();

        this->state = SwomProtocolCommunicatorState::WaitAcknowledge;
        tWaitAcknowledge.start(10s);

        const auto batches = qryGetMessagesBatches.handle(10);
        if (batches.isEmpty()) {
            return;
        }

        device.write(formatter.encodeTelFrame(batches));

        QList<MessagesBatchDto>::ConstIterator result;
        result = std::max_element(batches.begin(), batches.end(), [](auto a, auto b)->bool {
            return a.id < b.id;
        });

        cmdSetLastSentMessagesBatchId.handle(result->id);
    };

    cSendDataImmediately = QObject::connect(
        this, &SwomProtocolCommunicator::notifySendDataImmediatlyRequired,
        sendDataHandler
    );

    cSendDataReccurently = QObject::connect(
        &tSendDataReccurently, &QTimer::timeout,
        sendDataHandler
    );

    cWaitAcknowldege = QObject::connect(&tWaitAcknowledge, &QTimer::timeout, [&] {
        spdlog::warn("Waiting for acknowldege timed out");

        device.close();
    });

    requestAuthentication(device);
}

void
SwomProtocolCommunicator::interruptCommunication() {
    tWaitAcknowledge.stop();
    tSendDataReccurently.stop();

    QObject::disconnect(cReadData);
    QObject::disconnect(cSendDataImmediately);
    QObject::disconnect(cSendDataReccurently);
    QObject::disconnect(cWaitAcknowldege);
}

void
SwomProtocolCommunicator::sendDataImmediatly() {
    // emit notifySendDataImmediatlyRequired();
}

SwomProtocolCommunicatorState
SwomProtocolCommunicator::getCurrentState() const { return state; }

void
SwomProtocolCommunicator::requestAuthentication(QIODevice &device) {
    device.write(formatter.encodeAthFrame("104"));

    tWaitAcknowledge.start();

    state = SwomProtocolCommunicatorState::Authenticating;
}
