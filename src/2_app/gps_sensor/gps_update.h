#ifndef GPS_UPDATE_H
#define GPS_UPDATE_H

// qt
#include <QMetaType>
#include <QDateTime>

namespace Sensors::Gps {

/*!
 * A DTO object holds general GPS data.
 */
struct GpsUpdate {
    /*!
     * A UTC+0 datetime when GPS read has been produced.
     */
    const QDateTime& datetime { };

    const bool is_valid { false };

    const double latitude { 0.0 };

    const double longitude { 0.0 };

};

} // Sensors::Gps

Q_DECLARE_METATYPE(Sensors::Gps::GpsUpdate);

#endif // GPS_UPDATE_H
