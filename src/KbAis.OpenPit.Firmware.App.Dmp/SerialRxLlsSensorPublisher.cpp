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
    ,   m_spLlsDevice(this)
    ,   m_subLlsDeviceMessage { LlsDeviceMessage { } }
    ,   m_subLlsDeviceDiagInfo { LlsDeviceDiagInfo { } }
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
            if (m_decodeReplyResult.expectedRepliesAmount != 0) {
                PublishLlsDeviceMessage();
                PublishLlsDeviceDiagInfo();
            }

            m_decodeReplyResult.expectedRepliesAmount = 1;
            m_decodeReplyResult.recivedReplies.clear();

            RequestSingleRead();
        });

    const auto connected = m_spLlsDevice.open(QIODevice::ReadWrite);
    PLOGV_IF(connected) << "LLS publisher connected to /dev/ttyO2";
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
SerialRxLlsSensorPublisher::ConfigConnection() {
    // TODO: Get from configuration
    m_spLlsDevice.setPortName("/dev/ttyO2");
    m_spLlsDevice.setBaudRate(QSerialPort::Baud19200);
    m_spLlsDevice.setDataBits(QSerialPort::Data8);
    m_spLlsDevice.setStopBits(QSerialPort::OneStop);
    m_spLlsDevice.setFlowControl(QSerialPort::NoFlowControl);
}

void
SerialRxLlsSensorPublisher::HandleReadyRead() {
    const auto bytesToDecode = m_spLlsDevice.peek(1024);

    int scannedTotal { 0 };

    while (scannedTotal != bytesToDecode.length()) {
        auto [scanned, decodeResult] =
            OmnicommLlsProtocolFomratter::DecodeFirstReply(bytesToDecode);

        scannedTotal += scanned;

        if (!decodeResult) {
            auto error = decodeResult.error();

            // Not enough data for further processing, whait for next bytes batch
            if (error == OmnicommLlsProtocolFomratter::NotEnoughData) {
                return;
            }

            // Other kind of errors
            break;
        }

        m_decodeReplyResult.recivedReplies.push_back(*decodeResult);
    }

    m_spLlsDevice.read(scannedTotal);
}

void
SerialRxLlsSensorPublisher::RequestSingleRead() {
    // TODO: Get addresses from configuration
    std::vector<quint8> addresses { 0x01, 0x02, 0x03 };
    m_decodeReplyResult.expectedRepliesAmount = addresses.size();

    const auto encodedBytes = OmnicommLlsProtocolFomratter::EncodeOpSingleRead(addresses);
    if (m_spLlsDevice.write(encodedBytes)) {
        PLOGV << fmt::format("LLS publisher wrote {} byte(s)", encodedBytes.length());

        return;
    }

    // TODO: Publish diag info
    PLOGW << "LLS publisher could not send request on single read";
}

void SerialRxLlsSensorPublisher::PublishLlsDeviceMessage() {
    ranges::for_each(m_decodeReplyResult.recivedReplies, [](const LlsReplyReadData& x) {
        PLOGV << fmt::format("Got LLS data: {:d}, {:d}, {:d}, {:d}", x.Adr, x.Tem, x.Lvl, x.Frq);
    });
}

void SerialRxLlsSensorPublisher::PublishLlsDeviceDiagInfo() {

}
