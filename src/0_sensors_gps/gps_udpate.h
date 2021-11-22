#ifndef GPS_UDPATE_H
#define GPS_UDPATE_H

// qt
#include <QMetaType>
#include <QDateTime>

namespace Sensors::Gps {

struct GpsUpdate {

    const QDateTime& datetime { };

    const bool is_valid { false };

    const double latitude { 0.0 };

    const double longitude { 0.0 };

};

}

Q_DECLARE_METATYPE(Sensors::Gps::GpsUpdate);

#endif // GPS_UDPATE_H
