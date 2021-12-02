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
        this, &AuxRecurrentEventMapper::notifyEventPlaced,
        &collector, &RecurrentEventCollector::handleEventPlaced
    );

    connect(
        &gps_sensor, &GpsDeviceController::update_gps_data_signal,
        this, [&](const GpsUpdate& gps_update) {
            emit notifyEventPlaced(mapGpsUpdate(gps_update));
        }
    );
}

// TODO: make mappers reusable
const Event AuxRecurrentEventMapper::mapGpsUpdate(
    const GpsUpdate& gps_update
) {
    return {
        "GPS",
          QString::number(gps_update.latitude)
        + ","
        + QString::number(gps_update.longitude)
        + ","
        + gps_update.datetime.toString()
    };
};

