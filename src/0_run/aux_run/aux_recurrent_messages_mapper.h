#ifndef AUXRECURRENTMESSAGESMAPPER_H
#define AUXRECURRENTMESSAGESMAPPER_H

// qt
#include <QObject>
// qt deferred
#include <QLambdaThreadWorker>

#include "device_message.h"
#include "gps_device_controller.h"
#include "recurrent_messages_collector.h"

class AuxRecurrentEventMapper : public QObject {
    Q_OBJECT

public:
    explicit AuxRecurrentEventMapper(
        const RecurrentMessagesCollector& collector,
        const KbAis::Cfw::Sensors::Gps::SerialPortGpsDeviceController&,
        QObject *parent = nullptr
    );

private:
    Q_SIGNAL void notifyEventPlaced(const Message& event);

    const Message mapGpsUpdate(const KbAis::Cfw::Sensors::Gps::GpsUpdate& gps_update);

    QLambdaThreadWorker worker;

};

#endif // AUXRECURRENTMESSAGESMAPPER_H
