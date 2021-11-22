#ifndef GPS_DEVICE_CONTROLLER_H
#define GPS_DEVICE_CONTROLLER_H

#include <nmea/nmea_parser.h>

// std
#include <memory>
#include <vector>
// qt
#include <QObject>
#include <QThread>
// qt serial port
#include <QtSerialPort/QSerialPort>

// Contracts
namespace Sensors::Gps {

struct gps_update {

    double latitude;

    double longitude;

};

}

Q_DECLARE_METATYPE(Sensors::Gps::gps_update);

namespace Sensors::Gps {

class gps_device_controller : public QObject {
    Q_OBJECT;

public:
    gps_device_controller(QObject *parent = nullptr);

    ~gps_device_controller();

    /*!
     * \brief Emit singal on GPS data update from device.
     */
    Q_SIGNAL void update_gps_data_signal();

private:
    QSerialPort m_gps_device;

    std::vector<std::shared_ptr<Nmea::ISentence>> m_sentences;

    std::shared_ptr<Nmea::GgaSentence> m_gga_sentence;

    std::shared_ptr<Nmea::RmcSentence> m_rmc_sentence;  

    void setup_gps_device();

    bool reset_gps_device();

    void on_gps_device_read_ready();

};

} // Sensors::Gps
#endif // GPS_DEVICE_CONTROLLER_H
