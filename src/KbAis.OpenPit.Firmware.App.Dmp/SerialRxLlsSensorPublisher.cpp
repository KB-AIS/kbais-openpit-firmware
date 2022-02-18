#include "SerialRxLlsSensorPublisher.h"

// qt
#include <QtEndian>
// oss
#include <plog/Log.h>

#include "Format.h"
#include "Crc8Alogs.h"

using namespace std::chrono_literals;

constexpr std::chrono::duration LLS_READ_REQUEST_INTERVAL { 300ms };

constexpr qint8 LLS_REQ_PREFIX = 0x31;
constexpr qint8 LLS_REP_PREFIX = 0x3E;
constexpr qint8 LLS_SR_OP =  0x06;
constexpr qint8 LLS_SR_REP_LEN = 8;

SerialRxLlsSensorPublisher::SerialRxLlsSensorPublisher()
    : QObject()
{

}

void
SerialRxLlsSensorPublisher::StartWorking() {
    ConfigLlsDeviceConnection();

    QObject::connect(&m_spLlsDevice, &QIODevice::readyRead, this, &SerialRxLlsSensorPublisher::HandleReadyRead);

    // TODO: Add WAIT FOR REPLY TIMEOUT, send request only when got a reply
    QObject::connect(&m_tmLlsReadRequest, &QTimer::timeout, this, &SerialRxLlsSensorPublisher::HandleReadRequest);

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
        const auto replyBeningnPosition = bytes.indexOf(LLS_REP_PREFIX, bytesScanned);
        if (replyBeningnPosition == -1) {
            PLOGV << "LLS publisher could not find replay prefix";

            bytesScanned = bytes.length();
            break;
        }

        bytesScanned = replyBeningnPosition + 1;

        const auto llsSenderAddress = *reinterpret_cast<const quint8*>(bytes.constData() + replyBeningnPosition + 1);
        bytesScanned++;
        const auto llsOpCode        = *reinterpret_cast<const quint8*>(bytes.constData() + replyBeningnPosition + 2);
        bytesScanned++;

        if (llsOpCode != LLS_SR_OP) {
            PLOGV << "LLS publisher got unsupported reply";
            break;
        }

        if (bytes.mid(replyBeningnPosition).length() < 9) {
            return;
        }

        // TODO: User data reader
        const auto llsCelsiusTemp   = bytes.at(replyBeningnPosition + 3);
        const auto llsRelativeLevel = qFromLittleEndian<quint16>(bytes.mid(replyBeningnPosition + 4, 2).constData());
        const auto llsFrequency     = bytes.mid(replyBeningnPosition + 6).toUShort();
        const auto llsCrc           = *reinterpret_cast<const quint8*>(bytes.constData() + replyBeningnPosition + 8);
        bytesScanned += 6;

        PLOGD << fmt::format(
            "Got reply from LLS device: {:d}, {:d}, {:d}, {:d}, {:d}, {:d}"
        ,   llsSenderAddress, llsOpCode, llsCelsiusTemp, llsRelativeLevel, llsFrequency, llsCrc
        );
    }

    m_spLlsDevice.read(bytesScanned);
}

void
SerialRxLlsSensorPublisher::HandleReadRequest() {
    // TODO: Could have several sensors
    QByteArray l = QByteArray();
    l[0] = LLS_REQ_PREFIX;
    l[1] = 0x01; // TODO: FROM CONFIG
    l[2] = LLS_SR_OP;
    l[3] = calcCrc8Maxim(l);

    m_spLlsDevice.write(l);
}
