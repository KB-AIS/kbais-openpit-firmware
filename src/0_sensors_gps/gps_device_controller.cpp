#include "gps_device_controller.h"


#include "casting.h"

// std
#include <typeinfo>
#include <functional>
// plog
#include <plog/Log.h>
#include <QtGlobal>

namespace Sensors::Gps {

// !Q: where device name comes from?
const QString DEVICE_PORT_NAME { "/dev/ttyS5" };

GpsDeviceController::GpsDeviceController(QObject* parent) : QObject(parent),
    m_gps_device(),
    m_sentences() {

    connect(
        &m_gps_device, &QSerialPort::readyRead,

        this, &GpsDeviceController::on_gps_device_read_ready
    );

    setup_gps_device();
    reset_gps_device();
}

GpsDeviceController::~GpsDeviceController() {
    if (!m_gps_device.isOpen()) return;

    m_gps_device.close();
}

void GpsDeviceController::setup_gps_device() {
    m_gps_device.setDataBits(QSerialPort::Data8);
    m_gps_device.setFlowControl(QSerialPort::NoFlowControl);
    m_gps_device.setParity(QSerialPort::NoParity);
    m_gps_device.setPortName(DEVICE_PORT_NAME);
    m_gps_device.setStopBits(QSerialPort::OneStop);
}

bool GpsDeviceController::reset_gps_device() {
    m_gps_device.setBaudRate(QSerialPort::Baud9600);

    // TOOD: Reset power on pins
    auto is_device_open = m_gps_device.open(QSerialPort::ReadOnly);

    return is_device_open;
}

void GpsDeviceController::on_gps_device_read_ready() {
    // Remove sentences from read buffer
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

        // Skip update if GGA and RMC were not collected at the same time.
        if (m_gga_sentence->time != m_rmc_sentence->datetime.time()) {
            continue;
        }

        emit update_gps_data_signal(GpsUpdate {
            m_rmc_sentence->datetime,
            m_rmc_sentence->is_valid,
            m_rmc_sentence->lat,
            m_rmc_sentence->lon,
        });
    }
}

} // Sensors::Gps
