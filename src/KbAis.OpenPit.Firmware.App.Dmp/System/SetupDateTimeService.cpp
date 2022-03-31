#include "SetupDateTimeService.h"

// c
#include <time.h>
// std
#include <cmath>

SetupDateTimeService::SetupDateTimeService(const IRxGpsSensorPublisher& gpsSensorPublisher) {
    gpsSensorPublisher.get_observable()
        .subscribe(m_subscriptions, [this](const GpsMessage& x) { handle(x); });
}

SetupDateTimeService::~SetupDateTimeService() noexcept {
    m_subscriptions.unsubscribe();
}

void
SetupDateTimeService::handle(const GpsMessage& gpsMessage) {
    if (!gpsMessage.isValid) return;

   // Workaround: Using obsolet methods just to do the same thing as the original implementation.
   const time_t sysTime = QDateTime::currentDateTimeUtc().toTime_t();
   const time_t updTime = gpsMessage.datetime.toTime_t();

   if (std::abs(updTime - sysTime) <= 1) return;

   stime(&updTime);
}
