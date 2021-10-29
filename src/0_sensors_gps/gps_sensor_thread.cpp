#include "gps_sensor_thread.h"

using namespace Sensors::Ui;

gps_sensor_thread::gps_sensor_thread(QObject *parent) :
    QThread(parent)
{

}

void gps_sensor_thread::run() {
    auto setup_gps_device_serial_port = [](QSerialPort *device) {
        const QString device_port_name = "/dev/ttyS5";

        device->setPortName(device_port_name);
        device->setDataBits(QSerialPort::Data8);
        device->setStopBits(QSerialPort::OneStop);
        device->setParity(QSerialPort::NoParity);
        device->setFlowControl(QSerialPort::NoFlowControl);
    };

    m_gps_device_serial_port = new QSerialPort();
    setup_gps_device_serial_port(m_gps_device_serial_port);

    forever {
        auto return_code = exec();
    }
}
