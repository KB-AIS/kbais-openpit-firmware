#include "SwomProtocolCommunicator.h"

// qt
#include <QUuid>
// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

#include <Format.h>

// TODO: Move to fetcher interface
#include "Core/Persisting/Queries/SelectMessagesBatchesQry.h"
#include "Core/Persisting/Commands/UpdateSenderCmd.h"
#include "Core/Networking/Communicators/Swom/SwomProtocolFormatter.h"

using namespace std::chrono_literals;

// TODO: Pass equipment id with communicator configuration
static const QString EQUIPMENT_ID { "104" };

SwomProtocolCommunicator::SwomProtocolCommunicator() {

}

SwomProtocolCommunicator::~SwomProtocolCommunicator() {
    m_subs.unsubscribe();
}

void
SwomProtocolCommunicator::InitCommunication(QIODevice& device) {
    m_subs.unsubscribe();

    m_subs = rxcpp::composite_subscription();

    PLOGV << fmt::format("initiating communication session");

    rxqt::from_signal(&device, &QIODevice::readyRead).subscribe(m_subs, [&](auto) {
        constexpr quint32 BYTES_TO_PEEK = 1024;

        const auto byteToDecode = device.peek(BYTES_TO_PEEK);

        PLOGV << fmt::format("got data of {} bytes", byteToDecode.size());

        // TODO: Process bytes

        device.read(BYTES_TO_PEEK);
    });

    PerformAuthenticationRequest(device);
}

void
SwomProtocolCommunicator::StopCommunication() {
    m_subs.unsubscribe();
}

void
SwomProtocolCommunicator::PerformAuthenticationRequest(QIODevice& device) {
    const auto uuid = QUuid::createUuid();

    PLOGV << fmt::format("performing authentucation, message UUID is {}", uuid.toString());

    device.write(SwomProtocolFormatter::EncodeAthFrame(uuid, EQUIPMENT_ID));
}
