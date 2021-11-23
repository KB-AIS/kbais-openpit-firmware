#include "setup_time_handler.h"

// std
#include <cmath>
// qt
#include <QDateTime>

using namespace Sensors::Gps;

void SetupTimeHandler::handle_slot(const GpsUpdate& update) {
    if (!update.is_valid) {
        return;
    }

    const time_t sys_time = QDateTime::currentDateTimeUtc().toTime_t();
    const time_t upd_time = update.datetime.toTime_t();

    if (std::abs(upd_time - sys_time) <= 1) {
        return;
    }

    stime(&upd_time);
}
