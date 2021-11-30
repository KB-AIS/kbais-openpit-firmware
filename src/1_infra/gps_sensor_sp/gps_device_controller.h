#ifndef GPS_DEVICE_CONTROLLER_H
#define GPS_DEVICE_CONTROLLER_H

#include "gps_update.h"
#include "nmea/nmea_parser.h"

// std
#include <memory>
#include <vector>
// qt
#include <QObject>
#include <QtSerialPort/QSerialPort>

namespace Sensors::Gps {

class GpsDeviceController : public QObject {
    Q_OBJECT;

public:
    GpsDeviceController(QObject *parent = nullptr);

    ~GpsDeviceController();

    /*!
     * \brief Emit singal on GPS data update from device.
     */
    Q_SIGNAL void update_gps_data_signal(const GpsUpdate&);

private:
    QSerialPort m_gps_device;

    std::vector<std::shared_ptr<Nmea::NmeaSentence>> m_sentences;

    std::shared_ptr<Nmea::GgaSentence> m_gga_sentence;

    std::shared_ptr<Nmea::RmcSentence> m_rmc_sentence;  

    void setup_gps_device();

    bool reset_gps_device();

    void on_gps_device_read_ready();

};

} // Sensors::Gps
#endif // GPS_DEVICE_CONTROLLER_H
