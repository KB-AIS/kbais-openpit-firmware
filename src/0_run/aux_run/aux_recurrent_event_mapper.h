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
        const Sensors::Gps::GpsDeviceController& gpsSensor,
        QObject *parent = nullptr
    );

private:
    Q_SIGNAL void notifyEventPlaced(const Event& event);

    const Event mapGpsUpdate(const Sensors::Gps::GpsUpdate& gps_update);

    QLambdaThreadWorker worker;

};

#endif // AUXRECURRENTEVENTMAPPER_H
