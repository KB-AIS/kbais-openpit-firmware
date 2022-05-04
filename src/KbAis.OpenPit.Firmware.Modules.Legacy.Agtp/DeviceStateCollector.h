#ifndef DEVICESTATECOLLECTOR_H
#define DEVICESTATECOLLECTOR_H

// qt
#include <QtCore/qobject.h>

#include "opf/modules/sensors/crd/crd_sensor_publisher.h"
#include "opf/modules/sensors/gps/gps_sensor_publisher.h"

class DeviceStateCollector : public QObject {
    Q_OBJECT

    const i_gps_sensor_publisher& m_service_gps;

    const crd_sensor_publisher& m_service_tcr;

    rxcpp::composite_subscription m_subscriptions;

    gps_sensor_message m_cached_gps_msg;

    crd_sensor_message m_cached_tcr_msg;

public:
    DeviceStateCollector(
        const i_gps_sensor_publisher& service_gps
    ,   const crd_sensor_publisher& service_card_reader
    );

    void start(const rxqt::run_loop& loop);

    gps_sensor_message getGpsMessage() const;

    crd_sensor_message get_tcr_msg() const;

};

#endif // DEVICESTATECOLLECTOR_H
