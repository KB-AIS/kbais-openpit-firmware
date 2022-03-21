#include "SerialRxRcrSensorPublisher.h"

// std
#include <chrono>
// qt
#include <QDataStream>
// oss
#include <plog/Log.h>

#include "Crc16Alogs.h"
#include "Format.h"

using namespace std::chrono;

constexpr std::chrono::duration SEND_REQ_READ_CARD_NUMBER_INTERVAL { 500ms };

RxServiceCardReader::RxServiceCardReader()
    :   QObject()
    ,   m_serialport_cardreader(this)
    ,   m_timer_request_card_number(this)
    ,   m_publisher({ /* 'Пустое' сообщение */ })
{

}

void
RxServiceCardReader::start_work_on() {
    rxqt::from_signal(&m_serialport_cardreader, &QIODevice::readyRead)
        .subscribe([this](auto) { handle_ready_read(); });

    // TODO: use rxcpp::interval
    rxqt::from_signal(&m_timer_request_card_number, &QTimer::timeout).subscribe([this](auto) {
        publish_card_number();

        send_request_card_number();
    });

    config_device_connection();
    start_work_internal();
}

rxcpp::observable<CardReaderMessage>
RxServiceCardReader::get_observable() const {
    return m_publisher.get_observable();
}

void
RxServiceCardReader::config_device_connection() {
    m_serialport_cardreader.setPortName("/dev/ttyO2");
    m_serialport_cardreader.setBaudRate(QSerialPort::Baud19200);
    m_serialport_cardreader.setDataBits(QSerialPort::Data8);
    m_serialport_cardreader.setStopBits(QSerialPort::OneStop);
    m_serialport_cardreader.setFlowControl(QSerialPort::NoFlowControl);
}

void
RxServiceCardReader::start_work_internal() {
    if (!m_serialport_cardreader.open(QIODevice::ReadWrite)) {
        PLOGW << "Could not open connection to device";

        return;
    }

    m_timer_request_card_number.start(SEND_REQ_READ_CARD_NUMBER_INTERVAL);
}

void
RxServiceCardReader::publish_card_number() {
    if (!m_request_meta.response.has_value()) return;

    const auto now_time = steady_clock::now();

    const auto need_to_publish =
        m_publish_meta.first_uniqe_response != m_request_meta.response.value()
    ||  duration_cast<milliseconds>(now_time - m_publish_meta.first_uniqe_read_time) > 5s;

    if (!need_to_publish) return;

    m_publish_meta.first_uniqe_response = m_request_meta.response.value();
    m_publish_meta.first_uniqe_read_time = now_time;

    m_publisher.get_subscriber().on_next(CardReaderMessage {
        .card_reader_net_adr = m_request_meta.response->device_address
    ,   .card_number = m_request_meta.response->card_number
    });
}

void
RxServiceCardReader::send_request_card_number() {
    m_request_meta = { }; // Сброс предыдущих meta-данных

    PLOGV << fmt::format("Requesting to read card number from device {:x}", 0xF0);

    const auto bytes = FormatterModbusCardReader::encode_request_card_number(0xF0);
    const auto bytes_written = m_serialport_cardreader.write(bytes);

    if (bytes_written == -1) {
        PLOGW << fmt::format("Could not request to read card number from device {:x}", 0xF0);

        return;
    }

    PLOGV << fmt::format("Sent request to read card number from device {:x}", 0xF0);
}

void
RxServiceCardReader::handle_ready_read() {
    constexpr qint64 PEEK_SIZE { 16 };

    m_request_meta.got_any_data = true;

    auto bytes_peeked = m_serialport_cardreader.peek(PEEK_SIZE);

    const auto [examined, decode_result] = FormatterModbusCardReader::decode_response(bytes_peeked);

    m_request_meta.has_decode_rsp_err =
        decode_result.has_exception<FormatterModbusCardReader::DecodeResponseError>();

    if (decode_result) {
        PLOGD << "Got card number: " << decode_result->card_number;

        m_request_meta.response = std::move(decode_result.value());
    }

    m_serialport_cardreader.read(examined);
}
