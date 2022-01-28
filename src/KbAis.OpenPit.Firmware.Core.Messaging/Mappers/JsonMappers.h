#ifndef JSONMAPPERS_H
#define JSONMAPPERS_H

// Modules.Sensors.Gps
#include "GpsMessage.h"
// Utils.TrdParty.JsonQt
#include "JsonQt.h"

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GpsMessage
,   datetime
,   isValid
,   latitude
,   longitude
,   speed
,   gpsQauality
,   satellites
);

#endif // JSONMAPPERS_H
