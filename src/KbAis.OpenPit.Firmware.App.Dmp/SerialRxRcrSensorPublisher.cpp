#include "SerialRxRcrSensorPublisher.h"

// std
#include <chrono>
// qt
#include <QDataStream>
// oss
#include <plog/Log.h>

#include "Crc16Alogs.h"
#include "Format.h"
#include "FormatterModbusCardReader.h"

using namespace std::chrono_literals;

constexpr std::chrono::duration SEND_REQ_READ_CARD_NUMBER_INTERVAL { 500ms };

SerialRxRcrSensorPublisher::SerialRxRcrSensorPublisher()
    :   QObject()
    ,   m_sp_device(this)
    ,   m_tm_send_req_read_card_number(this)
{

}

void
SerialRxRcrSensorPublisher::start_work_on() {
    rxqt::from_signal(&m_sp_device, &QIODevice::readyRead)
        .subscribe([this](auto) { handle_ready_read(); });

    rxqt::from_signal(&m_tm_send_req_read_card_number, &QTimer::timeout)
        .subscribe([this](auto) { send_req_read_card_number(); });

    conf_device_connection();
    start_work_internal();
}

void
SerialRxRcrSensorPublisher::conf_device_connection() {
    m_sp_device.setPortName("/dev/ttyO2");
    m_sp_device.setBaudRate(QSerialPort::Baud19200);
    m_sp_device.setDataBits(QSerialPort::Data8);
    m_sp_device.setStopBits(QSerialPort::OneStop);
    m_sp_device.setFlowControl(QSerialPort::NoFlowControl);
}

void
SerialRxRcrSensorPublisher::start_work_internal() {
    if (!m_sp_device.open(QIODevice::ReadWrite)) {
        PLOGW << "Could not open connection to device";

        return;
    }

    m_tm_send_req_read_card_number.start(SEND_REQ_READ_CARD_NUMBER_INTERVAL);
}

void
SerialRxRcrSensorPublisher::send_req_read_card_number() {
    PLOGV << fmt::format("Requesting to read card number from device {:x}", 0xF0);

    const auto bytes = FormatterModbusCardReader::encode_req_read_card_number(0xF0);
    const auto bytes_written = m_sp_device.write(bytes);

    if (bytes_written == -1) {
        PLOGW << fmt::format("Could not request to read card number from device {:x}", 0xF0);

        return;
    }

    PLOGV << fmt::format("Sent request to read card number from device {:x}", 0xF0);
}

void
SerialRxRcrSensorPublisher::handle_ready_read() {
    auto peeked = m_sp_device.peek(32);
    if (peeked.length() >= 9) {
        PLOGD << peeked.toHex(':');
    }

    if (peeked.length() >= 9) {
        m_sp_device.readAll();
    }
}
