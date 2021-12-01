#ifndef AUXRECURRENTEVENTMAPPER_H
#define AUXRECURRENTEVENTMAPPER_H

#include "event.h"
#include "gps_device_controller.h"
#include "recurrent_event_collector.h"
// Qt
#include <QObject>
// QDeferred
#include <QLambdaThreadWorker>

class AuxRecurrentEventMapper : public QObject {
    Q_OBJECT

public:
    explicit AuxRecurrentEventMapper(
        const RecurrentEventCollector& collector,
        const Sensors::Gps::GpsDeviceController& gps_sensor,
        QObject *parent = nullptr
    );

private:
    Q_SIGNAL void place_event_signal(const Event& event);

    const Event map_gps_update_to_event(const Sensors::Gps::GpsUpdate& gps_update);

    QLambdaThreadWorker m_worker;

};

#endif // AUXRECURRENTEVENTMAPPER_H
