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

namespace KbAis::Cfw::Sensors::Gps {

class BaseGpsDeviceController : public QObject {
    Q_OBJECT

public:
    /*!
     * Emit singal on GPS data update from device.
     */
    Q_SIGNAL void notifyGpsDataUpdated(const GpsUpdate&);
};

class SerialPortGpsDeviceController : public BaseGpsDeviceController {
    Q_OBJECT

public:
    SerialPortGpsDeviceController();

    ~SerialPortGpsDeviceController();

private:
    QSerialPort* spGpsDevice;

    std::vector<std::shared_ptr<Nmea::NmeaSentence>> sentences;

    std::shared_ptr<Nmea::GgaSentence> ggaSentence;

    std::shared_ptr<Nmea::RmcSentence> rmcSentence;

    void setupGpsDevice();

    bool resetGpsDevice();

    void handleGpsDeviceReadyRead();

};

} // Sensors::Gps
#endif // GPS_DEVICE_CONTROLLER_H
