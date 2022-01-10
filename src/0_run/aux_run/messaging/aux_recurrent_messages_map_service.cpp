#include "aux_recurrent_messages_map_service.h"

// oss
#include <nlohmann/json.hpp>

#include "messaging/mappers/mapper_gps_update.h"
#include "utils/json_qt_support.h"

using json = nlohmann::json;

using namespace KbAis::Cfw::Sensors::Gps;

const QString MESSAGE_MONKIER_GPS_UPDATE { "GPS" };

AuxRecurrentMessagesMapService::AuxRecurrentMessagesMapService(
    const RecurrentMessagesCollector& collector,
    const GpsDeviceController& gpsDeviceController
) : QObject() {
    connect(
        this, &AuxRecurrentMessagesMapService::notifyIncommingMessageMapped,

        &collector, &RecurrentMessagesCollector::handleMessagePlaced
    );

    connect(
        &gpsDeviceController, &BaseGpsDeviceController::notifyGpsDataUpdated,

        this, [&](auto gpsUpdate) {
            const auto payload = json::to_msgpack(gpsUpdate);

            Message message {
                MESSAGE_MONKIER_GPS_UPDATE,
                QByteArray(reinterpret_cast<const char*>(payload.data()), payload.size()),
                QDateTime::currentDateTimeUtc(),
            };

            emit notifyIncommingMessageMapped(message);
        }
    );
}
