#include "crd_sensor_publisher.h"

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

crd_sensor_publisher::crd_sensor_publisher()
    :   QObject()
    ,   serialport_crd_(this)
    ,   timer_request_card_number_(this)
    ,   publisher_({ /* "Пустое" сообщение */ })
{
}

void
crd_sensor_publisher::start_work_on(const rxcpp::observe_on_one_worker& coord) {
    rxqt::from_signal(&serialport_crd_, &QIODevice::readyRead)
        .subscribe([this](auto) { handle_ready_read(); });

    rxcpp::observable<>::interval(SEND_REQ_READ_CARD_NUMBER_INTERVAL, coord)
        .subscribe([this](auto) {
            publish_card_number();

            send_request_card_number();
        });

    config_serialport_connection();

    if (!serialport_crd_.open(QIODevice::ReadWrite)) {
        PLOGW << "Could not open connection to device";

        return;
    }
}

rxcpp::observable<crd_sensor_message>
crd_sensor_publisher::get_observable(const rxcpp::observe_on_one_worker& coord) const {
    return publisher_.get_observable()
        .subscribe_on(coord);
}

void
crd_sensor_publisher::config_serialport_connection() {
    serialport_crd_.setPortName("/dev/ttyO2");
    serialport_crd_.setBaudRate(QSerialPort::Baud19200);
    serialport_crd_.setDataBits(QSerialPort::Data8);
    serialport_crd_.setStopBits(QSerialPort::OneStop);
    serialport_crd_.setFlowControl(QSerialPort::NoFlowControl);
}

void
crd_sensor_publisher::publish_card_number() {
    if (!request_meta_.response.has_value()) return;

    const auto now_time = steady_clock::now();

    const auto need_to_publish =
        publish_meta_.first_uniqe_response != request_meta_.response.value()
    ||  duration_cast<milliseconds>(now_time - publish_meta_.first_uniqe_read_time) > 5s;

    if (!need_to_publish) return;

    publish_meta_.first_uniqe_response = request_meta_.response.value();
    publish_meta_.first_uniqe_read_time = now_time;

    publisher_.get_subscriber().on_next(crd_sensor_message {
        .network_address = request_meta_.response->device_address
    ,   .card_number     = request_meta_.response->card_number
    });
}

void
crd_sensor_publisher::send_request_card_number() {
    request_meta_ = { }; // Сброс предыдущих meta-данных

    PLOGV << fmt::format("Requesting to read card number from device {:x}", 0xF0);

    const auto bytes = crd_formatter_tkcardreader_modbus::encode_request_card_number(0xF0);
    const auto bytes_written = serialport_crd_.write(bytes);

    if (bytes_written == -1) {
        PLOGW << fmt::format("Could not request to read card number from device {:x}", 0xF0);

        return;
    }

    PLOGV << fmt::format("Sent request to read card number from device {:x}", 0xF0);
}

void
crd_sensor_publisher::handle_ready_read() {
    constexpr qint64 PEEK_SIZE { 16 };

    request_meta_.got_any_data = true;

    auto bytes_peeked = serialport_crd_.peek(PEEK_SIZE);

    const auto [examined, decode_result] = crd_formatter_tkcardreader_modbus::decode_response(bytes_peeked);

    request_meta_.has_decode_rsp_err =
        decode_result.has_exception<crd_formatter_tkcardreader_modbus::DecodeResponseError>();

    if (decode_result) {
        PLOGD << "Got card number: " << decode_result->card_number;

        request_meta_.response = std::move(decode_result.value());
    }

    serialport_crd_.read(examined);
}
