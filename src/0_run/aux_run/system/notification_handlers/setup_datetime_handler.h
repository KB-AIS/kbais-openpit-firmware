#ifndef SETSYSTEMTIMECMDHANDLER_H
#define SETSYSTEMTIMECMDHANDLER_H

// qt
#include <QObject>

#include "gps_update.h"

namespace KbAis::Cfw::Core {

/*!
 * Used to setup system time by GPS update.
 */
class SetupDateTimerHandler : public QObject {
    Q_OBJECT

public:
    Q_SLOT virtual void handleGpsDataUdpated(const Sensors::Gps::GpsUpdate& update) = 0;
};

class SetupTimeHandlerImpl : public SetupDateTimerHandler {
    Q_OBJECT

public:
    Q_SLOT void handleGpsDataUdpated(const Sensors::Gps::GpsUpdate& update) override;

};

}

#endif // SETSYSTEMTIMECMDHANDLER_H
