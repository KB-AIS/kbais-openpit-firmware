#include "gps_sensor_thread.h"

// plog
#include <plog/Log.h>
// qdef
#include <QDeferred>
#include <QLambdaThreadWorker>

using namespace Sensors::Gps;

gps_sensor_thread::gps_sensor_thread(QObject *parent) : QThread(parent),
    m_gps_device(this) { }

gps_sensor_thread::~gps_sensor_thread() {
    // TODO: close m_gps_device
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
        this, [c_device = &m_gps_device] {
            PLOGD << "Reading from GPS device";

            auto data = c_device->readAll();

            if (data.isEmpty()) {
                PLOGD << "Recived nothing from GPS device";
                return;
            }

            // TODO: Use tiptruck NMEA parser
        }
    );

    PLOGI << "Setup GPS device in thread";
    setup_gps_device(m_gps_device);

    PLOGD << "Reset GPS device";
    reset_gps_device(m_gps_device);

    forever {
        emit update_gps_data_signal();

        auto exit_code = exec();
    }
}
