#ifndef DEVICESTATECOLLECTOR_H
#define DEVICESTATECOLLECTOR_H

// qt
#include <QObject>

#include "IRxGpsSensorPublisher.h"
#include "Modules/Sensors/Serials/TkCardReader/SerialRxRcrSensorPublisher.h"

class DeviceStateCollector : public QObject {
    Q_OBJECT

    const IRxGpsSensorPublisher& m_service_gps;

    const RxServiceCardReader& m_service_tcr;


    rxcpp::composite_subscription m_subscriptions;

    GpsMessage m_cached_gps_msg;

    CardReaderMessage m_cached_tcr_msg;

public:
    DeviceStateCollector(
        const IRxGpsSensorPublisher& service_gps
    ,   const RxServiceCardReader& service_card_reader
    );

    void start(const rxqt::run_loop& loop);

    GpsMessage getGpsMessage() const;

    CardReaderMessage get_tcr_msg() const;

};

#endif // DEVICESTATECOLLECTOR_H
