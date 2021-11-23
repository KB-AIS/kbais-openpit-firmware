#ifndef SETSYSTEMTIMECMDHANDLER_H
#define SETSYSTEMTIMECMDHANDLER_H


// sensors\gps
#include <gps_udpate.h>
// qt
#include <QObject>

class SetupTimeHandler : public QObject {
    Q_OBJECT

public:
    Q_SLOT void handle_slot(const Sensors::Gps::GpsUpdate&);

};

#endif // SETSYSTEMTIMECMDHANDLER_H
