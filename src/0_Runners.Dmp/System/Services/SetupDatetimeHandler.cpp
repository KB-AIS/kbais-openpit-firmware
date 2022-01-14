#include "SetupDatetimeHandler.h"

// c
#include <time.h>
// std
#include <cmath>
// qt
#include <QDateTime>

RxSetupDateTimeService::RxSetupDateTimeService(
    const IRxGpsSensorPublisher& gpsSensorPublisher
)
    : gpsMessageObservable { gpsSensorPublisher.getObservable() }
{
    start();
}

RxSetupDateTimeService::~RxSetupDateTimeService() {
    stop();
}

void
RxSetupDateTimeService::start() {
    subGpsSensorPublisher = rxcpp::composite_subscription();

    gpsMessageObservable.subscribe(subGpsSensorPublisher, SetupDateTimeHandler::handle);
}

void RxSetupDateTimeService::stop() {
    if (!subGpsSensorPublisher.is_subscribed()) return;

    subGpsSensorPublisher.unsubscribe();
}

void
SetupDateTimeHandler::handle(const GpsMessage& gpsMessage) {
    if (!gpsMessage.isValid) return;

    // Workaround: We using obsolet methods here just to do the same thing
    // as the original implementation.
    const time_t sysTime = QDateTime::currentDateTimeUtc().toTime_t();
    const time_t updTime = gpsMessage.datetime.toTime_t();

    if (std::abs(updTime - sysTime) <= 1) return;

    stime(&updTime);
}
