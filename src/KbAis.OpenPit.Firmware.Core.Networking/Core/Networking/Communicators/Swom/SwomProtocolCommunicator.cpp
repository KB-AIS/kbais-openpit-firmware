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

constexpr std::chrono::duration SEND_TEL_INTERVAL { 10s };

constexpr int MAX_MESSAGE_BATCHES_IN_FRAME { 4 };

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

    rxqt::from_signal(&m_tmEnequeReccur, &QTimer::timeout)
        .subscribe(m_subscriptions, [this, &d = device](auto) { SendTel(d); });

    SendAth(device);

    m_tmEnequeReccur.start(SEND_TEL_INTERVAL);
}

void
SwomProtocolCommunicator::StopCommunication() {
    m_subscriptions.unsubscribe();
}

void
SwomProtocolCommunicator::OnReadyRead(QIODevice &device) {
    constexpr quint32 BYTES_TO_PEEK = 1024;

    const auto byteToDecode = device.peek(BYTES_TO_PEEK);

    PLOGV << fmt::format("SWOM communicator got data of {} bytes", byteToDecode.size());

    device.read(BYTES_TO_PEEK);
}

void
SwomProtocolCommunicator::SendAth(QIODevice& device) {
    const auto uuid = QUuid::createUuid();

    PLOGV << "SWOM communicator is authenticating";

    const auto encodedAthPacket = SwomProtocolFormatter::EncodeAthPacket(uuid, EQUIPMENT_ID);

    device.write(SwomProtocolFormatter::EncodeFrame(encodedAthPacket));
}

void
SwomProtocolCommunicator::SendTel(QIODevice &device) {
    const auto messageBatches = SelectMessagesBatchesQry { }.handle(MAX_MESSAGE_BATCHES_IN_FRAME);

    if (messageBatches.isEmpty()) return;

    QByteArray encodedPackets;

    ranges::for_each(
        messageBatches
    |   ranges::views::transform([](const auto& x) {
            return SwomProtocolFormatter::EncodeTelPacket(QUuid::createUuid(), x);
        })
    ,   [&p = encodedPackets](auto&& x) { p.append(x); }
    );

    device.write(SwomProtocolFormatter::EncodeFrame(encodedPackets));

    const auto messageBatch =
        ranges::max_element(messageBatches, std::less<quint64>(), &MessagesBatchDto::id);

    UpdateSenderCmd { }.handle(messageBatch->id);
}
