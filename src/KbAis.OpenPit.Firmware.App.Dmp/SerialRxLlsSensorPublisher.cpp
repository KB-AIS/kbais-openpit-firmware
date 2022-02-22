#include "SerialRxLlsSensorPublisher.h"

// qt
#include <QtEndian>
// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

#include "Crc8Alogs.h"
#include "Format.h"

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
{

}

SerialRxLlsSensorPublisher::~SerialRxLlsSensorPublisher() {
    m_subsBag.unsubscribe();
}

void
SerialRxLlsSensorPublisher::StartPublishOn(const rxcpp::observe_on_one_worker& coordinator) {
    ConfigConnection();

    rxqt::from_signal(&m_spLlsDevice, &QIODevice::readyRead)
        .subscribe_on(coordinator)
        .subscribe(m_subsBag, [this](auto) { HandleReadyRead(); });

    rxcpp::observable<>::interval(SEND_REQUEST_INTERVAL, coordinator)
        .subscribe(m_subsBag, [this](auto) {
            if (m_decodeReplyResult.expectedReplies != 0) {
                PublishLlsDeviceMessage();
            }

            m_decodeReplyResult.replies.clear();

            RequestSingleRead();
        });

    const auto connected = m_spLlsDevice.open(QIODevice::ReadWrite);
    PLOGV_IF(connected) << "LLS publisher connected to /dev/ttyO2";
}

const rxcpp::observable<LlsDeviceMessage>
SerialRxLlsSensorPublisher::GetObservableMessage() const {
    return m_subLlsDeviceMessage.get_observable();
}

void
SerialRxLlsSensorPublisher::ConfigConnection() {
    m_spLlsDevice.setPortName("/dev/ttyO2");
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
            break;
        }

        // In case if there is not enough data for further processing, wait
        // for the next bytes batch.
        if (result.error() == OmnicommLlsProtocolFomratter::NotEnoughData) return;
    }

    m_spLlsDevice.read(consumed);
}

void
SerialRxLlsSensorPublisher::RequestSingleRead() {
    m_decodeReplyResult.expectedReplies = ADDRESSES.size();

    const auto encoded = OmnicommLlsProtocolFomratter::EncodeOpSingleRead(ADDRESSES);

    if (m_spLlsDevice.write(encoded)) {
        PLOGV << fmt::format("LLS publisher wrote {} byte(s)", encoded.length());
        return;
    }

    PLOGW << "LLS publisher could not send request on single read";
}

void SerialRxLlsSensorPublisher::PublishLlsDeviceMessage() {
    PLOGD << "LLS publisher got: " << m_decodeReplyResult.replies.size();
}
