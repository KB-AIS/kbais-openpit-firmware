#ifndef DEVICESTATECOLLECTOR_H
#define DEVICESTATECOLLECTOR_H

// qt
#include <QObject>

// Modules.Sensors.Gps
#include "IRxGpsSensorPublisher.h"
// Utils.TrdParty.RxQt
#include "RxQt.h"

class DeviceStateCollector : public QObject {
    Q_OBJECT

public:
    DeviceStateCollector(const IRxGpsSensorPublisher& gpsSensorPublisher);

    void start(const rxqt::run_loop& loop);

    GpsMessage getGpsMessage() const;

private:
    const IRxGpsSensorPublisher& mGpsSensorPublisher;

    rxcpp::composite_subscription mSubs;

    GpsMessage mCachedGpsMessage;

};

#endif // DEVICESTATECOLLECTOR_H
