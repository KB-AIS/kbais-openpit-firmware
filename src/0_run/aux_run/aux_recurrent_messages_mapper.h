#ifndef AUXRECURRENTMESSAGESMAPPER_H
#define AUXRECURRENTMESSAGESMAPPER_H

#include "device_message.h"
#include "gps_device_controller.h"
#include "recurrent_messages_collector.h"
// Qt
#include <QObject>
// QDeferred
#include <QLambdaThreadWorker>

class AuxRecurrentEventMapper : public QObject {
    Q_OBJECT

public:
    explicit AuxRecurrentEventMapper(
        const RecurrentMessagesCollector& collector,
        const Sensors::Gps::GpsDeviceController& gpsSensor,
        QObject *parent = nullptr
    );

private:
    Q_SIGNAL void notifyEventPlaced(const DeviceMessage& event);

    const DeviceMessage mapGpsUpdate(const Sensors::Gps::GpsUpdate& gps_update);

    QLambdaThreadWorker worker;

};

#endif // AUXRECURRENTMESSAGESMAPPER_H
