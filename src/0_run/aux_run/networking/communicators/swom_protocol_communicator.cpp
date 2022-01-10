#include "swom_protocol_communicator.h"

// oss
#include <nlohmann/json.hpp>
#include <plog/Log.h>

using namespace std::chrono_literals;

constexpr qint32 MESSAGES_BATCHES_QUEUE_CAPACITY { 100 };

constexpr qint32 MESSAGES_BATCHES_TO_PEEK { 5 };

constexpr std::chrono::milliseconds WAIT_ACKNOWLEDGE_TIMEOUT { 30s };

constexpr std::chrono::milliseconds ENEQUE_RECCURENTLY_INTERVAL { 10s };

SwomProtocolCommunicator::SwomProtocolCommunicator() :
        sendQueue { MESSAGES_BATCHES_QUEUE_CAPACITY, this }
    ,   timerWaitAcknowledge { this }
    ,   timerEnequeReccurently { this }
{
    timerWaitAcknowledge.setInterval(WAIT_ACKNOWLEDGE_TIMEOUT);

    cEnqueuReccurently = QObject::connect(
        &timerEnequeReccurently, &QTimer::timeout,
        this, &SwomProtocolCommunicator::enequeNextMessagesBatches);
}

SwomProtocolCommunicatorState
SwomProtocolCommunicator::state() const {
    return currentState;
}

void
SwomProtocolCommunicator::beginCommunication(QIODevice& device) {
    cReadData = QObject::connect(
        &device, &QIODevice::readyRead,
        [&] {
            timerWaitAcknowledge.stop();
            PLOGD << "Communicator has recived incomming bytes";

            constexpr quint32 BYTES_TO_PEEK = 1024;
            const auto bytesToDecode = device.peek(BYTES_TO_PEEK);

            if (state() == SwomProtocolCommunicatorState::Authenticating) {

            }

            if (state() == SwomProtocolCommunicatorState::WaitAcknowledge) {

            }

            device.readAll();
            sendQueue.requestPeek(MESSAGES_BATCHES_TO_PEEK);
        }
    );

    QObject::connect(
        &sendQueue, &MessagesBatchesSendQueue::notifyPeeked,
        [&](auto messagesBatches) {
            PLOGV << "Communicator peeked messages batches to send as a frame";

            QUuid frameUuid;
            device.write(formatter.encodeTelFrame(messagesBatches, frameUuid));

            timerWaitAcknowledge.start();
        }
    );
}

void
SwomProtocolCommunicator::endCommunication() { }

void
SwomProtocolCommunicator::requestSendImmediatly() {
    PLOGV << "Communicator has been requested to send immediatly";
    enequeNextMessagesBatches();
}

void
SwomProtocolCommunicator::requestAuthentication(QIODevice &device) {
    PLOGV << "Communicator has requested authentication";

    QUuid frameUuid;
    device.write(formatter.encodeAthFrame("104", frameUuid));

    currentState = SwomProtocolCommunicatorState::Authenticating;
    PLOGV << "Communicator's state changed to 'Authenticating'";
}

void
SwomProtocolCommunicator::enequeNextMessagesBatches() {
    if (sendQueue.remaningCapacity() == 0) {
        PLOGV << "Communicator has no room left for the next messages batches";
        // TODO: Schedule task to eneque as soon as batches dequeued
        return;
    }

    const auto messagesBatches =
        getMessagesBatchesQuery.handle(sendQueue.remaningCapacity());

    PLOGV << "Communicator took messages batches to enqueu: " << messagesBatches.size();
    if (messagesBatches.size() == 0) {
        PLOGV << "Nothing to enequeu";
        return;
    }

    sendQueue.enqueue(messagesBatches);
    PLOGV << "Communicator enqueued messages batches";
}

