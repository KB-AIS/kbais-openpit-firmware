#include "DeviceStateCollector.h"

// oss
#include <plog/Log.h>

DeviceStateCollector::DeviceStateCollector(
    const i_gps_sensor_publisher& service_gps
,   const crd_sensor_publisher& service_tcr
)
    :   QObject()
    ,   m_service_gps { service_gps }
    ,   m_service_tcr { service_tcr }
{ }

void DeviceStateCollector::start(const rxqt::run_loop& loop) {
    m_service_gps.get_observable()
        .observe_on(loop.observe_on_run_loop())
        .subscribe(
            m_subscriptions
        ,   [&](const GpsMessage& msg) {
                m_cached_gps_msg = msg;
            }
        );

    m_service_tcr.get_observable(loop.observe_on_run_loop())
        .observe_on(loop.observe_on_run_loop())
        .subscribe(
            m_subscriptions
        ,   [&](const crd_sensor_message& msg) {
                m_cached_tcr_msg = msg;
            }
        );
}

GpsMessage
DeviceStateCollector::getGpsMessage() const {
    return m_cached_gps_msg;
}

crd_sensor_message
DeviceStateCollector::get_tcr_msg() const {
    return m_cached_tcr_msg;
}
