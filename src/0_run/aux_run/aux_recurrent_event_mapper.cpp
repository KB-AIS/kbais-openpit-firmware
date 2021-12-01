#include "aux_recurrent_event_mapper.h"

// Qt
#include <QThread>
// plog
#include <plog/Log.h>

using namespace Sensors::Gps;

AuxRecurrentEventMapper::AuxRecurrentEventMapper(
    const RecurrentEventCollector& collector,
    const GpsDeviceController& gps_sensor,
    QObject* parent
): QObject(parent) {
    connect(
        this, &AuxRecurrentEventMapper::place_event_signal,

        &collector, &RecurrentEventCollector::place_event_slot
    );

    m_worker.startLoopInThread([&]() {
        PLOGD << "[LLS] Do work in thead: " << QThread::currentThread();

        emit place_event_signal({ "LLS", "test event" });
    }, 1 * 500);

    connect(
        &gps_sensor, &GpsDeviceController::update_gps_data_signal,

        this, [&](const GpsUpdate& gps_update) {
            PLOGD << "[GPS] Do work in thead: " << QThread::currentThread();

            emit place_event_signal(map_gps_update_to_event(gps_update));
        }
    );
}

const Event AuxRecurrentEventMapper::map_gps_update_to_event(
    const GpsUpdate& gps_update
)  {
    // todo: implement serialization
    return {
        "GPS",
          QString::number(gps_update.latitude)
        + ","
        + QString::number(gps_update.longitude)
        + ","
        + gps_update.datetime.toString()
    };
};

