#include "gps_sensor_thread.h"
// OSS
#include <plog/Log.h>

using namespace Sensors::Gps;

gps_sensor_thread::gps_sensor_thread(QObject *parent) :
    QThread(parent)
{

}

void gps_sensor_thread::run() {
    auto setup_gps_device = [](QSerialPort *device) {
        const QString device_port_name = "/dev/ttyS5";

        device->setPortName(device_port_name);
        device->setDataBits(QSerialPort::Data8);
        device->setStopBits(QSerialPort::OneStop);
        device->setParity(QSerialPort::NoParity);
        device->setFlowControl(QSerialPort::NoFlowControl);
    };

    auto reset_gps_device = []() {

    };

    PLOGI << "Setup GPS sensors thread";

    m_gps_device = new QSerialPort();
    setup_gps_device(m_gps_device);

    forever {
        emit update_gps_data_signal();

        auto return_code = exec();
    }
}
