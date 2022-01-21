#ifndef DEVICESTATECOLLECTOR_H
#define DEVICESTATECOLLECTOR_H

// qt
#include <QObject>

// cfw::infra::sensors::gps
#include "IRxGpsSensorPublisher.h"
// cfw::trdparty
#include "RxQt/RxQt.h"

class DeviceStateCollector : public QObject {
    Q_OBJECT

public:
    DeviceStateCollector(const IRxGpsSensorPublisher& gpsSensorPublisher);

    void startCollecting(const rxqt::run_loop& loop);

    // TODO: add getter
    GpsMessage mCachedGpsMessage;

private:
    const IRxGpsSensorPublisher& mGpsSensorPublisher;

    rxcpp::composite_subscription mSubs;

};

#endif // DEVICESTATECOLLECTOR_H
