#include "SerialRxLlsSensorPublisher.h"

// qt
#include <QtEndian>
// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

#include "Crc8Alogs.h"
#include "Format.h"

using namespace std::chrono_literals;

constexpr std::chrono::duration SEND_REQUEST_INTERVAL { 300ms };

SerialRxLlsSensorPublisher::SerialRxLlsSensorPublisher()
    :   QObject()
    ,   m_subLlsDeviceMessage { LlsDeviceMessage {} }
    ,   m_subLlsDeviceDiagInfo { LlsDeviceDiagInfo {} }
{

}

SerialRxLlsSensorPublisher::~SerialRxLlsSensorPublisher() {
    m_subsBag.unsubscribe();
}

void
SerialRxLlsSensorPublisher::StartWorkingOn() {
    ConfigLlsDeviceConnection();

    rxqt::from_signal(&m_spLlsDevice, &QIODevice::readyRead)
        .subscribe(m_subsBag, [this](auto) { HandleLlsReply(); });

    rxcpp::observable<>::interval(SEND_REQUEST_INTERVAL)
        .subscribe(m_subsBag, [this](auto) {
            if (m_decodeReplyResult.expectedLlsReplies != 0) {
                PublishLlsDeviceMessage();
                PublishLlsDeviceDiagInfo();
            }

            SendLlsRequestSingleRead();
        });

    m_spLlsDevice.open(QIODevice::ReadWrite);
}

rxcpp::observable<LlsDeviceMessage>
SerialRxLlsSensorPublisher::GetObservableLlsDeviceMessage() const {
    return m_subLlsDeviceMessage.get_observable();
}

rxcpp::observable<LlsDeviceDiagInfo>
SerialRxLlsSensorPublisher::GetObservableLlsDeviceDiagInfo() const {
    return m_subLlsDeviceDiagInfo.get_observable();
}

void
SerialRxLlsSensorPublisher::ConfigLlsDeviceConnection() {
    // TODO: Get from configuration
    m_spLlsDevice.setPortName("/dev/ttyO2");
    m_spLlsDevice.setBaudRate(QSerialPort::Baud19200);
    m_spLlsDevice.setDataBits(QSerialPort::Data8);
    m_spLlsDevice.setStopBits(QSerialPort::OneStop);
    m_spLlsDevice.setFlowControl(QSerialPort::NoFlowControl);
}

void
SerialRxLlsSensorPublisher::HandleLlsReply() {
    const auto bytesToDecode = m_spLlsDevice.peek(1024);

    int scannedTotal { 0 };

    while (scannedTotal != bytesToDecode.length()) {
        auto [scanned, decodeResult] =
            OmnicommLlsProtocolFomratter::DecodeNextReplySingleRead(bytesToDecode);

        scannedTotal += scanned;

        if (!decodeResult) {
            auto error = decodeResult.error();

            // Not enough data for further processing what for next;
            if (error == OmnicommLlsProtocolFomratter::NotEnoughData) {
                return;
            }

            // Other kind of error
            break;
        }
    }

    m_spLlsDevice.read(scannedTotal);
}

void
SerialRxLlsSensorPublisher::SendLlsRequestSingleRead() {
    // TODO: Get addresses from configuration
    std::vector<quint8> addresses { 0x01, 0x02, 0x03 };

    const auto encodedBytes = OmnicommLlsProtocolFomratter::EncodeOpSingleRead(addresses);

    if (m_spLlsDevice.write(encodedBytes)) {
        PLOGV << fmt::format("LLS publisher wrote {} byte(s)", encodedBytes.length());
        m_decodeReplyResult.expectedLlsReplies = addresses.size();
        return;
    }

    // TODO: Publish diag info
    PLOGW << "LLS publisher could not send request on single read";
}

void SerialRxLlsSensorPublisher::PublishLlsDeviceMessage() {

}

void SerialRxLlsSensorPublisher::PublishLlsDeviceDiagInfo() {

}
