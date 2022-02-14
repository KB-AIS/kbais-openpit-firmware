#include "SwomProtocolCommunicator.h"

// qt
#include <QUuid>
// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

#include "Core/Networking/Communicators/Swom/SwomProtocolFormatter.h"
#include "Format.h"

using namespace std::chrono_literals;

constexpr std::chrono::duration SEND_INTERVAL { 10s };

constexpr int MAX_MESSAGE_BATCHES_IN_FRAME { 4 };

// TODO: Take from configuration
static const QString EQUIPMENT_ID { "104" };

SwomProtocolCommunicator::SwomProtocolCommunicator() {

}

SwomProtocolCommunicator::~SwomProtocolCommunicator() {
    m_subscriptions.unsubscribe();
}

void
SwomProtocolCommunicator::InitCommunication(QIODevice& device) {
    m_subscriptions.unsubscribe();
    m_subscriptions = rxcpp::composite_subscription();

    PLOGV << fmt::format("Initiating SWOM communication session");

    rxqt::from_signal(&device, &QIODevice::readyRead)
        .subscribe(m_subscriptions, [this, &d = device](auto) { OnReadyRead(d); });

    rxqt::from_signal(&m_tmSendMessageBatches, &QTimer::timeout)
        .subscribe(m_subscriptions, [this, &d = device](auto) { HandleTelReq(d); });

    HandleAthReq(device);
}

void
SwomProtocolCommunicator::StopCommunication() {
    m_tmSendMessageBatches.stop();

    m_subscriptions.unsubscribe();
}

IProtocolCommunicator::NotificatorProtocolViolation_t
SwomProtocolCommunicator::GetObservableProtocolViolation() const {
    return m_sbjNotificatorProtocolViolation.get_observable();
}

void
SwomProtocolCommunicator::OnReadyRead(QIODevice& device) {
    // Naive implementaion without buffering, no need to refactor it for now.
    auto bytesToDecode = device.readAll();

    PLOGV << fmt::format("SWOM communicator got data of {} bytes", bytesToDecode.size());

    // Decode incomming bytes to acknowledge a sended requst.
    const auto decodeResult = SwomProtocolFormatter::DecodeAckFrame(bytesToDecode);

    if (!decodeResult) {
        std::string reason { fmt::format(
            "SWOM communicator could not parse incomming frame: {}"
        ,   decodeResult.error()
        )};
        return PublishProtocolViolation(std::move(reason));
    }

    if (m_currentState == SwomProtocolCommunicatorState::Authenticating) {
        return HandleAthRsp(decodeResult.value());
    }

    if (m_currentState == SwomProtocolCommunicatorState::WaitAcknowledge) {
        return HandleTelRsp(decodeResult.value(), device);
    }
}

void
SwomProtocolCommunicator::HandleAthReq(QIODevice& device) {
    m_currentState = SwomProtocolCommunicatorState::Authenticating;

    m_sndAthPacket = QUuid::createUuid();

    const auto encodedAthPacket =
        SwomProtocolFormatter::EncodeAthPacket(m_sndAthPacket, EQUIPMENT_ID);

    PLOGV << "SWOM communicator is performing authentication request ";

    const auto bytesWritten = device.write(SwomProtocolFormatter::EncodeFrame(encodedAthPacket));

    if (bytesWritten == -1) {
        return PublishProtocolViolation("SWOM communicator could not send authentication requset");
    }

    PLOGV << fmt::format("SWOM communicator send frame of {} bytes", bytesWritten);
}

void
SwomProtocolCommunicator::HandleAthRsp(const std::vector<SwomAckPacket>& ackPackets) {
    auto i = ranges::find(ackPackets, m_sndAthPacket, &SwomAckPacket::AckUuid);

    if (i == ranges::end(ackPackets)) {
        return PublishProtocolViolation("SWOM communicator did not recieve auth replay");
    }

    if (i->Result != SwomAckPacket::AckResultCode::Ok) {
        std::string reason {
            "SWOM communicator has been rejected to establish communication session"
        };
        return PublishProtocolViolation(reason);
    }

    PLOGV << "SWOM communicator has successfully authenticated";

    m_currentState = SwomProtocolCommunicatorState::ReadyToSend;
    m_tmSendMessageBatches.start(SEND_INTERVAL);
}

void
SwomProtocolCommunicator::HandleTelReq(QIODevice& device) {
    if (m_currentState != SwomProtocolCommunicatorState::ReadyToSend) return;

    const auto messageBatches = m_messageBatchGetAllHandler.handle(MAX_MESSAGE_BATCHES_IN_FRAME);
    if (messageBatches.isEmpty()) {
        PLOGV << "SWOM communicator got no collected messages to send";

        return;
    }

    QByteArray encodedPackets;
    ranges::for_each(
        messageBatches | ranges::views::transform([&](const auto& messageBatch) {
            // Create and stash meta information on a sending message batch.
            SwomTelPacketMeta meta { QUuid::createUuid(), messageBatch.id };
            m_sndTelPackets.push_back(meta);

            return SwomProtocolFormatter::EncodeTelPacket(meta.uuid, messageBatch);
        })
    ,   [&packets = encodedPackets](auto&& packet) { packets.append(packet); }
    );

    const auto bytesWritten = device.write(SwomProtocolFormatter::EncodeFrame(encodedPackets));

    if (bytesWritten == -1) {
        return PublishProtocolViolation("SWOM communicator could not send collected messages");
    }

    PLOGV << fmt::format("SWOM communicator send frame of {} bytes", bytesWritten);

    m_currentState = SwomProtocolCommunicatorState::WaitAcknowledge;
}

void
SwomProtocolCommunicator::HandleTelRsp(const std::vector<SwomAckPacket>& ackPackets, QIODevice& device) {
    const auto messageBatch =
        ranges::max_element(m_sndTelPackets, std::less<quint64>(), &SwomTelPacketMeta::messageBatchId);

    m_messageSenderUpdateCmd.handle(messageBatch->messageBatchId);

    m_sndTelPackets.clear();

    m_currentState = SwomProtocolCommunicatorState::ReadyToSend;
    HandleTelReq(device);
}

void
SwomProtocolCommunicator::PublishProtocolViolation(const std::string& message) {
    m_sbjNotificatorProtocolViolation.get_subscriber().on_next(ProtocolViolationNotif { message });
}
