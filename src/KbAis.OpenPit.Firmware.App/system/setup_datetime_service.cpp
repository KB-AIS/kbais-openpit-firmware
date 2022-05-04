#include "setup_datetime_service.h"

// c
#include <time.h>
// std
#include <ctime>

using namespace std::chrono;
using namespace std::chrono_literals;

setup_datetime_service::setup_datetime_service(
    const i_gps_sensor_publisher& gps_sensor_publisher
) {
    gps_sensor_publisher.get_observable()
        .subscribe(subscriptions_, [](const gps_sensor_message& x) { handle_gps_message(x); });
}

setup_datetime_service::~setup_datetime_service() noexcept {
    subscriptions_.unsubscribe();
}

void setup_datetime_service::handle_gps_message(const gps_sensor_message& msg) {
    if (!msg.is_valid) return;

    const auto sys_dt = QDateTime::currentDateTimeUtc();

    if (!need_sync_system_datetime(msg.datetime, sys_dt)) return;

    sync_system_datetime(msg.datetime);
}

bool setup_datetime_service::need_sync_system_datetime(
    const QDateTime& gps_datetime, const QDateTime& sys_datetime
) {
    constexpr duration ACCEPTABLE_DIFF { 1s };

    const auto diff = seconds { std::abs(gps_datetime.secsTo(sys_datetime)) };

    return diff >= ACCEPTABLE_DIFF;
}

void setup_datetime_service::sync_system_datetime(const QDateTime& datetime) {
    seconds      sec_part  { datetime.toSecsSinceEpoch() };
    milliseconds ms { datetime.toMSecsSinceEpoch() };
    nanoseconds  nsec_part { ms - sec_part };

    const timespec t  {
        .tv_sec  = static_cast<__time_t>(sec_part.count())
    ,   .tv_nsec = static_cast<__syscall_slong_t>(nsec_part.count())
    };
    clock_settime(CLOCK_REALTIME, &t);
}
