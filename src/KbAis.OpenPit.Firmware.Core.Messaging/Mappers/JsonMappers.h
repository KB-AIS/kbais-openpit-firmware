#ifndef JSONMAPPERS_H
#define JSONMAPPERS_H

// Modules.Sensors.Gps
#include "GpsMessage.h"
// Utils.TrdParty.JsonQt
#include "JsonQt.h"

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GpsMessage
,   datetime
,   is_valid
,   latitude
,   longitude
,   speed
);

#endif // JSONMAPPERS_H
