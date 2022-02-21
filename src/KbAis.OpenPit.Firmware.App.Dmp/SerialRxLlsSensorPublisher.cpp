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

SerialRxLlsSensorPublisher::SerialRxLlsSensorPublisher(
    IRxConfigurationChangePublisher& configurationPublisher
)
    :   m_configurationPublisher(configurationPublisher)
    ,   m_spLlsDevice(this)
    ,   m_subLlsDeviceMessage(LlsDeviceMessage { })
    ,   m_subLlsDeviceDiagInfo(LlsDeviceHelth { })
{

}

SerialRxLlsSensorPublisher::~SerialRxLlsSensorPublisher() {
    m_subsBag.unsubscribe();
}

int random_number(int N) // random value in [0, N)
{
    static std::random_device seed;
    static std::mt19937 eng(seed());
    std::uniform_int_distribution<> dist(0, N - 1);
    return dist(eng);
}

void
SerialRxLlsSensorPublisher::StartPublishOn(const rxcpp::observe_on_one_worker& coordinator) {

    m_configurationPublisher.getChangeObservable("networking")
        .sample_with_time(500ms, coordinator)
        .subscribe(m_subsBag, [this](AppConfiguration confiugration) {
            m_spLlsDevice.close();

            const auto portIdx = random_number(10) % 2;
            ConfigConnection(portIdx);

            const auto connected = m_spLlsDevice.open(QIODevice::ReadWrite);
            PLOGV_IF(connected) << "LLS publisher connected to " << portIdx;
        });

    rxqt::from_signal(&m_spLlsDevice, &QIODevice::readyRead)
        .subscribe_on(coordinator)
        .subscribe(m_subsBag, [this](auto) { HandleReadyRead(); });

    rxcpp::observable<>::interval(SEND_REQUEST_INTERVAL, coordinator)
        .subscribe(m_subsBag, [this](auto) {
            if (m_decodeReplyResult.expectedRepliesAmount != 0) {
                PublishLlsDeviceMessage();
                PublishLlsDeviceDiagInfo();
            }

            m_decodeReplyResult.recivedReplies.clear();

            RequestSingleRead();
        });
}

const rxcpp::observable<LlsDeviceMessage>
SerialRxLlsSensorPublisher::GetObservableLlsDeviceMessage() const {
    return m_subLlsDeviceMessage.get_observable();
}

rxcpp::observable<LlsDeviceHelth>
SerialRxLlsSensorPublisher::GetObservableHelthCheck() const {
    return m_subLlsDeviceDiagInfo.get_observable();
}

void
SerialRxLlsSensorPublisher::ConfigConnection(int idx) {
    m_spLlsDevice.setPortName(idx == 1 ? "/dev/ttyO2" : "/dev/ttySC2");
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
    // TODO: Get addresses from configuration
    std::vector<quint8> addresses { 0x01, 0x02, 0x03 };

    PLOGD << "LLS publisher got: " << m_decodeReplyResult.recivedReplies.size();
}

void SerialRxLlsSensorPublisher::PublishLlsDeviceDiagInfo() {

}
