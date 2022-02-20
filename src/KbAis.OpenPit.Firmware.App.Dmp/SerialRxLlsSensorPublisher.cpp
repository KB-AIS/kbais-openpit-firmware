#include "SerialRxLlsSensorPublisher.h"

// qt
#include <QtEndian>
// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

#include "Format.h"
#include "Crc8Alogs.h"

using namespace std::chrono_literals;

constexpr std::chrono::duration LLS_READ_REQUEST_INTERVAL { 300ms };

constexpr quint8 LLS_REQ_PREFIX = 0x31;
constexpr quint8 LLS_REP_PREFIX = 0x3E;

constexpr quint8 LLS_OP_CODE_SINGLEREAD     = 0x06;
constexpr int    LLS_OP_REPL_SINGLEREAD_LEN = 9;

SerialRxLlsSensorPublisher::SerialRxLlsSensorPublisher()
    : QObject()
{

}

void
SerialRxLlsSensorPublisher::StartWorking() {
    ConfigLlsDeviceConnection();

    QObject::connect(&m_spLlsDevice, &QIODevice::readyRead, this, &SerialRxLlsSensorPublisher::HandleReadyRead);

    // TODO: Add WAIT FOR REPLY TIMEOUT, send request only when got a reply
    QObject::connect(&m_tmLlsReadRequest, &QTimer::timeout, [&]() {
        HandleRequestSingleRead({ 0x01, 0x02, 0x03 });
    });

    m_spLlsDevice.open(QIODevice::ReadWrite);

    m_tmLlsReadRequest.start(LLS_READ_REQUEST_INTERVAL);
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
SerialRxLlsSensorPublisher::HandleReadyRead() {
    const auto bytes = m_spLlsDevice.peek(1024);

    if (bytes.length() <= 3) {
        PLOGV << "LLS publisher got not enough data to process reply";

        return;
    }

    int bytesScanned { 0 };

    while (bytesScanned != bytes.length()) {
        const auto repStartIdx = bytes.indexOf(LLS_REP_PREFIX, bytesScanned);

        if (repStartIdx == -1) {
            PLOGV << "LLS publisher could not find replay prefix";

            bytesScanned = bytes.length();
            break;
        }

        bytesScanned = repStartIdx + 1;

        const auto bytesPtr = bytes.constData();

        const auto llsAdr = *reinterpret_cast<const quint8*>(bytesPtr + repStartIdx + 1);
        const auto llsOpc = *reinterpret_cast<const quint8*>(bytesPtr + repStartIdx + 2);

        bytesScanned += 2;

        if (llsOpc != LLS_OP_CODE_SINGLEREAD) {
            PLOGW << "LLS publisher got unsupported reply";

            break;
        }

        // Check if publisher got enough data to perfrom parsing
        if (bytes.mid(repStartIdx).length() < LLS_OP_REPL_SINGLEREAD_LEN) {
            return;
        }

        const auto llsCrc = *reinterpret_cast<const quint8*>(bytesPtr + repStartIdx + 8);
        if (calcCrc8Maxim(bytes.mid(repStartIdx, 8)) != llsCrc) {
            // TODO: Publish DiagInfo error: got wrong crc
            break;
        };

        LlsMessage msg {
            bytes.at(repStartIdx + 3)
        ,   qFromLittleEndian<quint16>(bytesPtr + repStartIdx + 4)
        ,   qFromLittleEndian<quint16>(bytesPtr + repStartIdx + 6)
        };
        bytesScanned += 6;

        PLOGD << fmt::format(
            "Got reply from LLS device: {:d}, {:d}, {:d}, {:d}, {:d}, {:d}"
        ,   llsAdr, llsOpc, msg.Tem, msg.Lvl, msg.Frq, llsCrc
        );
    }

    m_spLlsDevice.read(bytesScanned);
}

void
SerialRxLlsSensorPublisher::HandleRequestSingleRead(
    std::vector<quint8> addresses
) {
    QByteArray llsRequestSingleRead;

    int currentByte = 0;

    ranges::for_each(addresses, [&](auto address) {
        llsRequestSingleRead[currentByte] = LLS_REQ_PREFIX;
        llsRequestSingleRead[currentByte + 1] = address;
        llsRequestSingleRead[currentByte + 2] = LLS_OP_CODE_SINGLEREAD;
        llsRequestSingleRead[currentByte + 3] =
            calcCrc8Maxim(llsRequestSingleRead.mid(currentByte, currentByte + 3));

        currentByte += 4;
    });

    m_spLlsDevice.write(llsRequestSingleRead);
}
