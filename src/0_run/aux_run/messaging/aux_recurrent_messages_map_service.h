#ifndef AUX_RECURRENT_MESSAGES_MAP_SERVICE_H
#define AUX_RECURRENT_MESSAGES_MAP_SERVICE_H

// qt
#include <QObject>

#include "gps_device_controller.h"
#include "messaging/collectors/recurrent_messages_collector.h"
#include "messaging/message.h"

using GpsDeviceController = KbAis::Cfw::Sensors::Gps::BaseGpsDeviceController;
using GpsUpdate           = KbAis::Cfw::Sensors::Gps::GpsUpdate;

class AuxRecurrentMessagesMapService : public QObject {
    Q_OBJECT

public:
    AuxRecurrentMessagesMapService(
        const RecurrentMessagesCollector& messagesCollector,
        const GpsDeviceController& gpsDeviceController
    );

private:
    Q_SIGNAL void notifyIncommingMessageMapped(const Message& message);
};

#endif // AUX_RECURRENT_MESSAGES_MAP_SERVICE_H
