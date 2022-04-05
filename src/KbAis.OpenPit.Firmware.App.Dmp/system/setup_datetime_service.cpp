#include "setup_datetime_service.h"

// c
#include <time.h>
// std
#include <cmath>

SetupDateTimeService::SetupDateTimeService(const IRxGpsSensorPublisher& gps_publisher) {
    gps_publisher.get_observable()
        .subscribe(subscriptions_, [this](const GpsMessage& x) { handle(x); });
}

SetupDateTimeService::~SetupDateTimeService() noexcept {
    subscriptions_.unsubscribe();
}

void
SetupDateTimeService::handle(const GpsMessage& gps_message) {
    if (!gps_message.isValid) return;

   // Используется obsolete-метод для повторения реализации с прошивки TK
   const time_t sys_time = QDateTime::currentDateTimeUtc().toTime_t();
   const time_t new_time = gps_message.datetime.toTime_t();

   if (std::abs(new_time - sys_time) <= 1) return;

   stime(&new_time);
}
