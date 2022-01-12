#ifndef GPSUPDATEDTO_H
#define GPSUPDATEDTO_H

// qt
#include <QDateTime>

struct GpsUpdateDto {

    QDateTime datetime;

    bool isValid { false };

    double latitude { 0.0 };

    double longitude { 0.0 };

    double speed { 0.0 };

    short gpsQauality { 0 };

    short satellites { 0 };

};

#endif // GPSUPDATEDTO_H
