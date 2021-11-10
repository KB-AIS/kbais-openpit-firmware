#ifndef GPS_DEVICE_CONTOLLER_H
#define GPS_DEVICE_CONTOLLER_H

#include <nmea/nmea_parser.h>

// std
#include <memory>
// qt
#include <QObject>
#include <QThread>
#include <QVector>
// qt serial port
#include <QtSerialPort/QSerialPort>

namespace Sensors::Gps {

/*!
 * \brief Thread to fetch data from GPS serial divice.
 */
class gps_device_controller : public QObject {
    Q_OBJECT;

public:
    gps_device_controller(QObject *parent = nullptr);

    ~gps_device_controller();

private:
    void setup_gps_device();

    bool reset_gps_device();

    void on_gps_device_read_ready();

private:
    QSerialPort m_gps_device;

    QVector<std::shared_ptr<Nmea::ISentence>> m_sentences;

    std::shared_ptr<Nmea::GgaSentence> m_gga_sentence;

    std::shared_ptr<Nmea::RmcSentence> m_rmc_sentence;

signals:
    /*!
     * \brief Emit singal on GPS data update from device.
     */
    void update_gps_data_signal();

};

} // Sensors::Gps
#endif // GPS_DEVICE_CONTOLLER_H
