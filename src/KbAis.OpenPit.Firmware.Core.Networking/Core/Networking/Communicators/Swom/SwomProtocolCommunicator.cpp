#include "SwomProtocolCommunicator.h"

// qt
#include <QUuid>
// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

// TODO: Move to fetcher interface
#include "Core/Persisting/Queries/SelectMessagesBatchesQry.h"
#include "Core/Persisting/Commands/UpdateSenderCmd.h"
#include "Core/Networking/Communicators/Swom/SwomProtocolFormatter.h"

using namespace std::chrono_literals;

SwomProtocolCommunicator::SwomProtocolCommunicator() {

}

SwomProtocolCommunicator::~SwomProtocolCommunicator() {
    m_subs.unsubscribe();
}

void
SwomProtocolCommunicator::InitCommunication(QIODevice& device) {
    m_subs = rxcpp::composite_subscription();

    rxqt::from_signal(&device, &QIODevice::readyRead).subscribe(m_subs, [&](auto) {
        constexpr quint32 BYTES_TO_PEEK = 1024;

        const auto byteToDecode = device.peek(BYTES_TO_PEEK);

        auto uuid = QUuid::fromRfc4122(byteToDecode);
        PLOGD << "Got ACK UUID: " << uuid.toString();

        device.read(BYTES_TO_PEEK);
    });

    rxqt::from_signal(&m_tEnequeReccur, &QTimer::timeout).subscribe(m_subs, [&](auto) {
        const auto messageBatches = SelectMessagesBatchesQry { }.handle(10);

        if (messageBatches.isEmpty()) return;

        device.write(SwomProtocolFormatter::EncodeTelFrame(messageBatches));

        const auto messageBatch =
            ranges::max_element(messageBatches, std::greater<quint64>(), &MessagesBatchDto::id);

        UpdateSenderCmd { }.handle(messageBatch->id);
    });

    device.write(SwomProtocolFormatter::EncodeAckFrame("104"));

    m_tEnequeReccur.start(10s);
}

void
SwomProtocolCommunicator::StopCommunication() {
    PLOGD << "SWOM STOP";
}
