#ifndef GPS_UPDATE_H
#define GPS_UPDATE_H

// qt
#include <QMetaType>
#include <QDateTime>

namespace KbAis::Cfw::Sensors::Gps {

/*!
 * A DTO object holds general GPS data.
 */
struct GpsUpdate {
    /*!
     * A UTC+0 datetime when GPS read has been produced.
     */
    QDateTime datetime;

    bool isValid;

    double latitude;

    double longitude;

};

} // Sensors::Gps

Q_DECLARE_METATYPE(KbAis::Cfw::Sensors::Gps::GpsUpdate);

#endif // GPS_UPDATE_H
