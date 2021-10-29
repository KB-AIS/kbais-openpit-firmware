#ifndef GPS_SENSOR_THREAD_H
#define GPS_SENSOR_THREAD_H

#include <QObject>
#include <QThread>
#include <QtSerialPort/QSerialPort>

namespace Sensors::Ui {

class gps_sensor_thread : public QThread {
    Q_OBJECT;

public:
    gps_sensor_thread(QObject *parent = nullptr);

    void run() override;

//public slots:
//    void update_gps_data_slot();

private:
    QSerialPort *m_gps_device_serial_port;
};

}
#endif // GPS_SENSOR_THREAD_H
