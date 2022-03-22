#include "DeviceStateCollector.h"

// oss
#include <plog/Log.h>

DeviceStateCollector::DeviceStateCollector(
    const IRxGpsSensorPublisher& service_gps
,   const RxServiceCardReader& service_tcr
)
    :   QObject()
    ,   m_service_gps { service_gps }
    ,   m_service_tcr { service_tcr }
{ }

void DeviceStateCollector::start(const rxqt::run_loop& loop) {
    m_service_gps.GetObservable()
        .observe_on(loop.observe_on_run_loop())
        .subscribe(
            m_subscriptions
        ,   [&](const GpsMessage& msg) {
                m_cached_gps_msg = msg;
            }
        );

    m_service_tcr.get_observable()
        .observe_on(loop.observe_on_run_loop())
        .subscribe(
            m_subscriptions
        ,   [&](const CardReaderMessage& msg) {
                m_cached_tcr_msg = msg;
            }
        );
}

GpsMessage
DeviceStateCollector::getGpsMessage() const {
    return m_cached_gps_msg;
}

CardReaderMessage
DeviceStateCollector::get_tcr_msg() const {
    return m_cached_tcr_msg;
}
