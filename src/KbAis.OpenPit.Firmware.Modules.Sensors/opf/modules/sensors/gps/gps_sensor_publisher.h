#ifndef GPS_SENSOR_PUBLISHER_H
#define GPS_SENSOR_PUBLISHER_H

// std
#include <memory>
#include <vector>
// qt
#include <QtCore/qobject.h>
#include <QtSerialPort/qserialport.h>
//oss
#include <rxcpp/rx.hpp>

#include "opf/modules/sensors/gps/nmea/NmeaParser.h"

struct gps_sensor_message {

    QDateTime datetime;

    bool is_valid { false };

    double latitude { 0.0 };

    double longitude { 0.0 };

    double speed { 0.0 };

    short gps_quality { 0 };

    short satellites { 0 };

};

class i_gps_sensor_publisher {

public:
    virtual ~i_gps_sensor_publisher() noexcept = default;

    virtual const rxcpp::observable<gps_sensor_message> get_observable() const = 0;

};

class gps_sensor_publisher : public i_gps_sensor_publisher, public QObject {

    QSerialPort serialGpsSensor;

    rxcpp::subjects::behavior<gps_sensor_message> subject;

    std::vector<std::shared_ptr<NmeaSentence>> nmeaSentences;

    std::shared_ptr<GgaSentence> lastGgaSentence;

    std::shared_ptr<RmcSentence> lastRmcSentence;

    void SetupGpsDevice();

    bool ResetGpsDevice();

    void handle_ready_read();

    bool NeedPublishGpsMessage() const;

    gps_sensor_message map_gps_senosr_message() const;

public:
    gps_sensor_publisher();

    ~gps_sensor_publisher();

    const rxcpp::observable<gps_sensor_message> get_observable() const override;

};

#endif // GPS_SENSOR_PUBLISHER_H
