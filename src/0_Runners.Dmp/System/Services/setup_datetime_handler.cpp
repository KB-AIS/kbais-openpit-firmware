#include "setup_datetime_handler.h"

// std
#include <cmath>
// qt
#include <QDateTime>

RxSetupDateTimeService::RxSetupDateTimeService(
    const IRxGpsSensorPublisher& gpsSensorPublisher
)
    : gpsUpdateObservable { gpsSensorPublisher.getObservable() } {}

RxSetupDateTimeService::~RxSetupDateTimeService() {
    stop();
}

void
RxSetupDateTimeService::start() {
    subGpsSensorPublisher = rxcpp::composite_subscription();

    gpsUpdateObservable.subscribe(subGpsSensorPublisher, SetupDateTimeHandler::handle);
}

void RxSetupDateTimeService::stop() {
    if (!subGpsSensorPublisher.is_subscribed()) return;

    subGpsSensorPublisher.unsubscribe();
}

void
SetupDateTimeHandler::handle(const GpsUpdateDto& gpsUpdate) {
    if (!gpsUpdate.isValid) return;

    // Workaround: We using obsolet methods here just to do the same thing
    // as the original implementation.
    const time_t sysTime = QDateTime::currentDateTimeUtc().toTime_t();
    const time_t updTime = gpsUpdate.datetime.toTime_t();

    if (std::abs(updTime - sysTime) <= 1) return;

    stime(&updTime);
}
