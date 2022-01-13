#ifndef JSONMAPPERS_H
#define JSONMAPPERS_H

// cfw::infra::sensors::gps
#include "GpsUpdateDto.h"

// cfw::trdparty
#include "JsonQt/JsonQt.h"

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
