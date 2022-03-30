#include "SerialRxLcsSensorPublisher.h"

using namespace std::chrono_literals;

constexpr std::chrono::duration SEND_REQUEST_INTERVAL { 300ms };

SerialRxLcsSensorPublisher::SerialRxLcsSensorPublisher()
    :   QObject()
    ,   m_device(this)
{

}

SerialRxLcsSensorPublisher::~SerialRxLcsSensorPublisher() noexcept {

}

void
SerialRxLcsSensorPublisher::start_publish_on(const rxcpp::observe_on_one_worker& worker) {
    handle_device_configure();

    rxqt::from_signal(&m_device, &QIODevice::readyRead)
        .subscribe_on(worker)
        .subscribe([this](auto) { handle_device_ready_read(); });

    rxcpp::observable<>::interval(SEND_REQUEST_INTERVAL, worker)
        .subscribe([this](auto) { handle_send_request(); });

    const auto connected = m_device.open(QIODevice::ReadWrite);
}

void
SerialRxLcsSensorPublisher::handle_device_configure() {
    m_device.setPortName("/dev/ttyO2");
    m_device.setBaudRate(QSerialPort::Baud19200);
    m_device.setDataBits(QSerialPort::Data8);
    m_device.setStopBits(QSerialPort::OneStop);
    m_device.setFlowControl(QSerialPort::NoFlowControl);
}

void
SerialRxLcsSensorPublisher::handle_device_ready_read() {

}

void
SerialRxLcsSensorPublisher::handle_send_request() {

}
