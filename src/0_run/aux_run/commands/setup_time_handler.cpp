#include "setup_time_handler.h"

// std
#include <cmath>

// qt
#include <QDateTime>
#include <plog/Log.h>

using namespace Sensors::Gps;

void SetupTimeHandler::handle_slot(const GpsUpdate& update) {
    if (!update.is_valid) {
        return;
    }

    // Workaorund: We using obsolet methods here just to do the same thing
    // as the original implementation.
    const time_t sys_time = QDateTime::currentDateTimeUtc().toTime_t();
    const time_t upd_time = update.datetime.toTime_t();

    if (std::abs(upd_time - sys_time) <= 1) {
        return;
    }

    // !Q: Maybe this's not a good idea to change system time directly?
    stime(&upd_time);
}
