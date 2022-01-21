#include "DeviceStateCollector.h"

// oss
#include <plog/Log.h>

DeviceStateCollector::DeviceStateCollector(
    const IRxGpsSensorPublisher &gpsSensorPublisher
)
    : QObject()
    , mGpsSensorPublisher { gpsSensorPublisher }
{

}

void DeviceStateCollector::startCollecting(const rxqt::run_loop& loop) {
    mGpsSensorPublisher.getObservable()
        .observe_on(loop.observe_on_run_loop())
        .subscribe(mSubs, [&](const GpsMessage& msg) {
            mCachedGpsMessage = msg;
        });
}
