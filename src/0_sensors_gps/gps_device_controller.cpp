#include <gps_device_controller.h>

// utils
#include <casting.h>

// std
#include <typeinfo>
#include <functional>
// plog
#include <plog/Log.h>
#include <QtGlobal>

namespace Sensors::Gps {

constexpr int PEEK_SIZE { 1024 };

// !QUESTION: where device name comes from?
const QString DEVICE_PORT_NAME = "/dev/ttyS5";

gps_device_controller::gps_device_controller(QObject* parent) : QObject(parent),
    m_gps_device(),
    m_sentences() {

    connect(
        &m_gps_device, &QSerialPort::readyRead,

        this, &gps_device_controller::on_gps_device_read_ready
    );

    setup_gps_device();
    reset_gps_device();
}

gps_device_controller::~gps_device_controller() {
    if (!m_gps_device.isOpen()) return;

    m_gps_device.close();
}

void gps_device_controller::setup_gps_device() {
    m_gps_device.setDataBits(QSerialPort::Data8);
    m_gps_device.setFlowControl(QSerialPort::NoFlowControl);
    m_gps_device.setParity(QSerialPort::NoParity);
    m_gps_device.setPortName(DEVICE_PORT_NAME);
    m_gps_device.setStopBits(QSerialPort::OneStop);
}

bool gps_device_controller::reset_gps_device() {
    m_gps_device.setBaudRate(QSerialPort::Baud9600);

    // TOOD: Reset power on pins
    auto is_device_open = m_gps_device.open(QSerialPort::ReadOnly);

    return is_device_open;
}

void gps_device_controller::on_gps_device_read_ready() {
    // PLOGD << "Consuming from GPS device";

    m_sentences.clear();

    Nmea::process_input(m_gps_device, m_sentences);

    for (auto& sentence : m_sentences) {
        Utils::Casting::action_if<Nmea::GgaSentence>(sentence, [=](auto gga_sentence) {
            m_gga_sentence = gga_sentence;
        });

        Utils::Casting::action_if<Nmea::RmcSentence>(sentence, [=](auto rmc_sentence) {
            m_rmc_sentence = rmc_sentence;
        });

        if (!m_gga_sentence || !m_rmc_sentence) {
            continue;
        }

        PLOGD << m_gga_sentence->time.toString();
        PLOGD << m_rmc_sentence->datetime.toString();

        emit update_gps_data_signal();
    }
}

} // Sensors::Gps
