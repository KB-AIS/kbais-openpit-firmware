#include "lcs_sensor_publisher.h"

using namespace std::chrono_literals;

constexpr std::chrono::duration SEND_REQUEST_INTERVAL { 300ms };

QString DEFAULT_PORT_NAME { QStringLiteral("/dev/ttyO2") };

lcs_sensor_publisher::lcs_sensor_publisher()
    :   QObject()
    ,   serial_port_lcs_(this)
{
}

void
lcs_sensor_publisher::start_work_on(const rxcpp::observe_on_one_worker& coord) {
    config_serialport_connection();

    rxqt::from_signal(&serial_port_lcs_, &QIODevice::readyRead)
        .subscribe_on(coord)
        .subscribe([this](auto) { handle_ready_read(); });

    rxcpp::observable<>::interval(SEND_REQUEST_INTERVAL, coord)
        .subscribe([](auto) {  });

    /*const auto connected = */serial_port_lcs_.open(QIODevice::ReadWrite);
}

rxcpp::observable<lcs_sensor_message>
lcs_sensor_publisher::get_observable(const rxcpp::observe_on_one_worker& coord) const {

}

void
lcs_sensor_publisher::config_serialport_connection() {
    serial_port_lcs_.setPortName(DEFAULT_PORT_NAME);
    serial_port_lcs_.setBaudRate(QSerialPort::Baud19200);
    serial_port_lcs_.setDataBits(QSerialPort::Data8);
    serial_port_lcs_.setStopBits(QSerialPort::OneStop);
    serial_port_lcs_.setFlowControl(QSerialPort::NoFlowControl);
}

void
lcs_sensor_publisher::handle_ready_read() {

}
