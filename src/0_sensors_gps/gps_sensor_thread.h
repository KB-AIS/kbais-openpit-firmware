#ifndef GPS_SENSOR_THREAD_H
#define GPS_SENSOR_THREAD_H

#include <nmea/nmea_parser.h>

// std
#include <memory>
// qt
#include <QObject>
#include <QThread>
#include <QVector>
// qt serial port
#include <QtSerialPort/QSerialPort>

using std::shared_ptr;

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
    void process_device_read();

    /*!
     * \brief Serial port to read data from GPS device.
     */
    QSerialPort m_gps_device;

    QVector<shared_ptr<Nmea::ISentence>> m_sentences;

    shared_ptr<Nmea::GgaSentence> m_gga_sentence;

    shared_ptr<Nmea::RmcSentence> m_rmc_sentence;

signals:
    /*!
     * \brief Emit singal on GPS data update from device.
     */
    void update_gps_data_signal();

};

} // Sensors::Gps
#endif // GPS_SENSOR_THREAD_H
