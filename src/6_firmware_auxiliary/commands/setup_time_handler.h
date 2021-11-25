#ifndef SETSYSTEMTIMECMDHANDLER_H
#define SETSYSTEMTIMECMDHANDLER_H


#include "gps_udpate.h"

// qt
#include <QObject>

/*!
 * Used to setup system time by GPS update.
 */
class SetupTimeHandler : public QObject {
    Q_OBJECT

public:
    Q_SLOT void handle_slot(const Sensors::Gps::GpsUpdate&);

};

#endif // SETSYSTEMTIMECMDHANDLER_H
