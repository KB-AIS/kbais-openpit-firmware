#ifndef GPSMESSAGE_H
#define GPSMESSAGE_H

// qt
#include <QtCore/qdatetime.h>

struct GpsMessage {

    QDateTime datetime;

    bool is_valid { false };

    double latitude { 0.0 };

    double longitude { 0.0 };

    double speed { 0.0 };

    short gpsQauality { 0 };

    short satellites { 0 };

};

#endif // GPSMESSAGE_H
