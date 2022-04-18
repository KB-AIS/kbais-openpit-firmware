#include "setup_datetime_service.h"

// c
#include <time.h>
// std
#include <ctime>

SetupDateTimeService::SetupDateTimeService(const i_gps_sensor_publisher& gps_sensor_publisher) {
    gps_sensor_publisher.get_observable()
        .subscribe(subscriptions_, [this](const GpsMessage& x) { handle(x); });
}

SetupDateTimeService::~SetupDateTimeService() noexcept {
    subscriptions_.unsubscribe();
}

void
SetupDateTimeService::handle(const GpsMessage& msg) {
    if (!msg.isValid) return;

    auto diff = msg.datetime.secsTo(QDateTime::currentDateTime());

    if (std::abs(diff) <= 1) return;

    std::chrono::seconds      s  { msg.datetime.toSecsSinceEpoch() };
    std::chrono::milliseconds ms { msg.datetime.toMSecsSinceEpoch() };

    std::chrono::nanoseconds  ns { ms - s };

    const timespec time_to_set  {
        .tv_sec  = static_cast<__time_t>(s.count())
    ,   .tv_nsec = static_cast<__syscall_slong_t>(ns.count())
    };
    clock_settime(CLOCK_REALTIME, &time_to_set);
}
