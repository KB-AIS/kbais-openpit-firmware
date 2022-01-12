#ifndef JSONMAPPERS_H
#define JSONMAPPERS_H

// oss
#include <nlohmann/json.hpp>

#include "GpsUpdateDto.h"
#include "Utils/NlohmannJsonQtSupport.h"

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    GpsUpdateDto,
    datetime,
    isValid,
    latitude,
    longitude,
    speed,
    gpsQauality,
    satellites
);

#endif // JSONMAPPERS_H
