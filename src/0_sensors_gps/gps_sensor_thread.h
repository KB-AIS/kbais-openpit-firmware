#ifndef GPS_SENSOR_THREAD_H
#define GPS_SENSOR_THREAD_H

// Qt
#include <QObject>
#include <QThread>
// Qt SerialPort
#include <QtSerialPort/QSerialPort>

namespace Sensors::Gps {

/*!
 * \brief Thread to fetch data from GPS serial divice.
 */
class gps_sensor_thread : public QThread {
    Q_OBJECT;

public:
    gps_sensor_thread(QObject *parent = nullptr);
    ~gps_sensor_thread();
    void run() override;

private:
    /*!
     * \brief Serial port to read data from GPS device.
     */
    QSerialPort m_gps_device;

signals:
    /*!
     * \brief Emit singal on GPS data update from device.
     */
    void update_gps_data_signal();

};

} // Sensors::Gps
#endif // GPS_SENSOR_THREAD_H
