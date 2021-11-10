#include <gps_sensor_thread.h>

// utils
#include <casting.h>

// std
#include <typeinfo>
#include <functional>
// plog
#include <plog/Log.h>
#include <QtGlobal>

namespace Sensors::Gps {

const int PEEK_SIZE { 1024 };

gps_sensor_thread::gps_sensor_thread(QObject *parent) : QThread(parent),
    m_gps_device(), m_sentences() { }

gps_sensor_thread::~gps_sensor_thread() {
    m_gps_device.close();
}

void gps_sensor_thread::run() {
    auto setup_gps_device = [](QSerialPort &device) {
        // !QUESTION: where device name comes from?
        const QString device_port_name = "/dev/ttyS5";

        device.setDataBits(QSerialPort::Data8);
        device.setFlowControl(QSerialPort::NoFlowControl);
        device.setParity(QSerialPort::NoParity);
        device.setPortName(device_port_name);
        device.setStopBits(QSerialPort::OneStop);
    };

    auto reset_gps_device = [](QSerialPort &device) {
        device.setBaudRate(QSerialPort::Baud9600);

        // TOOD: Reset power on pins
        auto isDeviceOpen = device.open(QSerialPort::ReadOnly);

        return isDeviceOpen;
    };

    connect(
        &m_gps_device, &QSerialPort::readyRead,

        this, &gps_sensor_thread::process_device_read
    );

    PLOGD << "Setup GPS device in thread";
    setup_gps_device(m_gps_device);

    PLOGD << "Reset GPS device";
    reset_gps_device(m_gps_device);

    forever {
        auto exit_code = exec();
    }
}

void gps_sensor_thread::process_device_read() {
    PLOGD << "Consuming from GPS device";
    // Clear previous output recived from GPS device
    m_sentences.clear();

    Nmea::process_input(m_gps_device, m_sentences);

    for (auto& sentence : m_sentences) {
        Utils::Casting::action_if<Nmea::GgaSentence>(sentence, [=](auto gga_sentence) {
            PLOGD << "Capture GGA sentence";
            m_gga_sentence = gga_sentence;
        });

        Utils::Casting::action_if<Nmea::RmcSentence>(sentence, [=](auto mc_sentence) {
            PLOGD << "Capture RMC sentence";
            m_rmc_sentence = mc_sentence;
        });

        if (!m_gga_sentence || !m_rmc_sentence) continue;

        emit update_gps_data_signal();
    }
}

} // Sensors::Gps
