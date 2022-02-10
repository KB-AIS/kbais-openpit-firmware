#include "SwomProtocolCommunicator.h"

// qt
#include <QUuid>
// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

#include "Core/Networking/Communicators/Swom/SwomProtocolFormatter.h"
#include "Core/Persisting/Commands/UpdateSenderCmd.h"
#include "Core/Persisting/Queries/SelectMessagesBatchesQry.h"
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

    HandleAthReq(device);
}

void
SwomProtocolCommunicator::StopCommunication() {
    m_subscriptions.unsubscribe();
}

void
SwomProtocolCommunicator::HandleAthReq(QIODevice& device) {
    m_currentState = SwomProtocolCommunicatorState::Authenticating;
    m_sndAthPacket = { QUuid::createUuid(), SwomPacketType::Ath };

    const auto encodedAthPacket =
        SwomProtocolFormatter::EncodeAthPacket(m_sndAthPacket.uuid, EQUIPMENT_ID);

    PLOGV << "SWOM communicator is performing authentication request ";

    const auto hasFrameBeenWritten =
        device.write(SwomProtocolFormatter::EncodeFrame(encodedAthPacket));
    if (hasFrameBeenWritten != -1) return;

    PLOGW << "SWOM communicator could not send authentication requset";
    // TODO: Emit 'shutdown communication' request
}

void
SwomProtocolCommunicator::HandleAthRsp(const std::vector<SwomAckPacket>& ackPackets) {
    auto i = ranges::find(ackPackets, m_sndAthPacket.uuid, &SwomAckPacket::AckUuid);

    if (i == ranges::end(ackPackets)) {
        PLOGW << "SWOM communicator did not recieve auth replay";
        // TODO: Emit 'shutdown communication' request
        return;
    }

    if (i->Result != SwomAckPacket::AckResultCode::Ok) {
        PLOGW << "SWOM communicator has been rejected to establish communication session";
        // TODO: Emit 'shutdown communication' request
        return;
    }

    PLOGV << "SWOM communicator has successfully authenticated";
    // TODO: Call start data sending
}


void
SwomProtocolCommunicator::OnReadyRead(QIODevice& device) {
    // TODO: Naive implementaion without buffering
    auto bytesToDecode = device.readAll();

    PLOGV << fmt::format("SWOM communicator got data of {} bytes", bytesToDecode.size());

    const auto decodeResult = SwomProtocolFormatter::DecodeAckFrame(bytesToDecode);

    if (!decodeResult) {
        PLOGW << fmt::format("SWOM communicator could not parse incomming frame: {}", decodeResult.error());
        // TODO: Emit 'shutdown communication' request
        return;
    }

    if (m_currentState == Authenticating) {
        return HandleAthRsp(decodeResult.value());
    }
}

void
SwomProtocolCommunicator::SendCollectedMessages(QIODevice& device) {

}
