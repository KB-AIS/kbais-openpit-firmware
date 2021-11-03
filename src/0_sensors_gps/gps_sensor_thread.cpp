#include <gps_sensor_thread.h>
#include <utils/nmea_parser.h>

// plog
#include <plog/Log.h>
// qdef
#include <QDeferred>
#include <QLambdaThreadWorker>

namespace Sensors::Gps {

const int PEEK_SIZE { 1024 };

gps_sensor_thread::gps_sensor_thread(QObject *parent) : QThread(parent),
    m_gps_device() { }

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

    bool is_frame_open = false;

    connect(
        &m_gps_device, &QSerialPort::readyRead,
        this, [&] {
            PLOGD << "Consuming from GPS device";

            Nmea::process_input(m_gps_device);

            PLOGD << "Finish consuming from GPS device";
        }
    );

    PLOGD << "Setup GPS device in thread";
    setup_gps_device(m_gps_device);

    PLOGD << "Reset GPS device";
    reset_gps_device(m_gps_device);

    forever {
        emit update_gps_data_signal();

        auto exit_code = exec();
    }
}

} // Sensors::Gps
