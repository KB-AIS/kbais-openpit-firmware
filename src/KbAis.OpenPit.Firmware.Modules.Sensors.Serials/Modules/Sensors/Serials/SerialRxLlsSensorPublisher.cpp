#include "SerialRxLlsSensorPublisher.h"

// qt
#include <QtEndian>
// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

#include "Crc8Alogs.h"
#include "Format.h"
#include "RxQt.h"

using namespace std::chrono_literals;

constexpr std::chrono::duration SEND_REQUEST_INTERVAL { 2s };

// TODO: Get from configuration
const std::vector<quint8> ADDRESSES { 0x01, 0x02, 0x03 };

SerialRxLlsSensorPublisher::SerialRxLlsSensorPublisher(
    IRxConfigurationChangePublisher& configurationPublisher
)
    :   m_configurationPublisher(configurationPublisher)
    ,   m_spLlsDevice(this)
    ,   m_subLlsDeviceMessage(LlsDeviceMessage { })
    ,   m_subLlsDeviceHealth(LlsDeviceHealth { })
{

}

SerialRxLlsSensorPublisher::~SerialRxLlsSensorPublisher() {
    m_subsBag.unsubscribe();
}

void
SerialRxLlsSensorPublisher::start_publish_on(const rxcpp::observe_on_one_worker& coordinator) {
    ConfigConnection();

    rxqt::from_signal(&m_spLlsDevice, &QIODevice::readyRead)
        .subscribe_on(coordinator)
        .subscribe(m_subsBag, [this](auto) { HandleReadyRead(); });

    rxcpp::observable<>::interval(SEND_REQUEST_INTERVAL, coordinator)
        .subscribe(m_subsBag, [this](auto) {
            if (m_decodeReplyResult.expectedReplies != 0) {
                PublishLlsDeviceMessage();
            }
            RequestSingleRead();
        });

    const auto connected = m_spLlsDevice.open(QIODevice::ReadWrite);
    PLOGV_IF(connected) << "LLS publisher connected to /dev/ttyO2";
    PublishLlsDeviceMessage();
}

const rxcpp::observable<LlsDeviceMessage>
SerialRxLlsSensorPublisher::GetObservableMessage() const {
    return m_subLlsDeviceMessage.get_observable();
}

const rxcpp::observable<LlsDeviceHealth>
SerialRxLlsSensorPublisher::GetObservableHealthStatus() const {
    return m_subLlsDeviceHealth.get_observable();
}

void
SerialRxLlsSensorPublisher::ConfigConnection() {
    m_spLlsDevice.setPortName("/dev/ttyO4");
    m_spLlsDevice.setBaudRate(QSerialPort::Baud19200);
    m_spLlsDevice.setDataBits(QSerialPort::Data8);
    m_spLlsDevice.setStopBits(QSerialPort::OneStop);
    m_spLlsDevice.setFlowControl(QSerialPort::NoFlowControl);
}

void
SerialRxLlsSensorPublisher::HandleReadyRead() {
    const auto bytesToDecode = m_spLlsDevice.peek(1024);
    auto consumed = 0;

    while (consumed != bytesToDecode.length()) {
        auto [examined, result] = OmnicommLlsProtocolFomratter::DecodeFirstReply(bytesToDecode);

        consumed += examined;

        // On success cache reply and move to next.
        if (result) {
            m_decodeReplyResult.replies.push_back(*result);
            continue;
        }

        // In case if there is not enough data for further processing, wait
        // for the next bytes batch.
        if (result.error() == OmnicommLlsProtocolFomratter::NotEnoughData) return;

        PublishLlsDeviceHealth(LlsDeviceError::InvalidData);
    }

    m_spLlsDevice.read(consumed);
}

void
SerialRxLlsSensorPublisher::RequestSingleRead() {
    m_decodeReplyResult.expectedReplies = ADDRESSES.size();
    m_decodeReplyResult.replies.clear();

    const auto encoded = OmnicommLlsProtocolFomratter::EncodeOpSingleRead(ADDRESSES);

    if (m_spLlsDevice.write(encoded)) {
        PLOGV << fmt::format("LLS publisher requested for {} replies", ADDRESSES.size());
        return;
    }

    PLOGW << "LLS publisher could not send request on single read";
    PublishLlsDeviceHealth(LlsDeviceError::FailedToSendRequest);
}

void
SerialRxLlsSensorPublisher::PublishLlsDeviceMessage() {
    const auto numberOfReplies { m_decodeReplyResult.replies.size() };

    PLOGD << fmt::format("LLS publisher got {} replies", numberOfReplies);

    auto data = ADDRESSES
    |   ranges::views::transform([](quint8 x) { return std::make_pair(x, std::nullopt); })
    |   ranges::to<LlsDeviceCombinedData_t>();

    if (numberOfReplies == 0) {
        PublishLlsDeviceHealth(LlsDeviceError::NoData);
        return m_subLlsDeviceMessage.get_subscriber().on_next(LlsDeviceMessage { data });
    }

    uint32_t numberOfUniqeReplies = ranges::distance(
        m_decodeReplyResult.replies
    |   ranges::views::unique([](const LlsReplyReadData& a, const LlsReplyReadData& b) {
            return a.Adr == b.Adr;
        })
    );

    if (numberOfUniqeReplies < ADDRESSES.size()) {
        PublishLlsDeviceHealth(LlsDeviceError::PartialData);
    }

    ranges::for_each(
        m_decodeReplyResult.replies
    ,   [&d = data](const LlsReplyReadData& x) { d[x.Adr] = x; }
    );

    m_subLlsDeviceMessage.get_subscriber().on_next(LlsDeviceMessage { data });
}

void
SerialRxLlsSensorPublisher::PublishLlsDeviceHealth(std::optional<LlsDeviceError> error) {
    auto isConnected = m_spLlsDevice.isOpen();

    m_subLlsDeviceHealth.get_subscriber().on_next(LlsDeviceHealth { isConnected, error });
}
