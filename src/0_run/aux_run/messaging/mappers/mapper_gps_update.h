#ifndef MAPPER_GPS_UPDATE_H
#define MAPPER_GPS_UPDATE_H

// oss
#include <nlohmann/json.hpp>

#include "gps_update.h"
#include "utils/mapper_qt_types.h"

namespace KbAis::Cfw::Sensors::Gps {

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GpsUpdate, datetime, isValid, latitude, longitude);

}

#endif // MAPPER_GPS_UPDATE_H
