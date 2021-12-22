#ifndef MAPPER_GPS_UPDATE_H
#define MAPPER_GPS_UPDATE_H

// oss
#include <nlohmann/json.hpp>

#include "gps_update.h"
#include "utils/json_qt_support.h"

namespace KbAis::Cfw::Sensors::Gps {

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GpsUpdate, datetime, isValid, latitude, longitude);

}

#endif // MAPPER_GPS_UPDATE_H
