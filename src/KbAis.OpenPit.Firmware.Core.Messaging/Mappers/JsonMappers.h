#ifndef JSONMAPPERS_H
#define JSONMAPPERS_H

// cfw::infra::sensors::gps
#include "GpsMessage.h"

// cfw::trdparty
#include "JsonQt.h"

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    GpsMessage,
    datetime,
    isValid,
    latitude,
    longitude,
    speed,
    gpsQauality,
    satellites
);

#endif // JSONMAPPERS_H
