#include "aux_recurrent_messages_mapper.h"

// Qt
#include <QThread>
// OSS
#include <plog/Log.h>
#include <json.hpp>

using json = nlohmann::json;

using namespace Sensors::Gps;

namespace Sensors::Gps {

void to_json(json& j, const GpsUpdate& x) {
    j = json {
        { "datetime", x.datetime.toString().toStdString() },
        { "is_valid", x.is_valid },
        { "latitude", x.latitude },
        { "longitude", x.longitude },
    };
}

void from_json(const json& j, GpsUpdate& x) {
    std::string dateTimeStr {};

    j.at("datetime").get_to(dateTimeStr);
    x.datetime = QDateTime::fromString(QString::fromStdString(dateTimeStr));
    j.at("is_valid").get_to(x.is_valid);
    j.at("latitude").get_to(x.latitude);
    j.at("longitude").get_to(x.longitude);
}

}

AuxRecurrentEventMapper::AuxRecurrentEventMapper(
    const RecurrentMessagesCollector& collector,
    const GpsDeviceController& gps_sensor,
    QObject* parent
): QObject(parent) {
    connect(
        this, &AuxRecurrentEventMapper::notifyEventPlaced,
        &collector, &RecurrentMessagesCollector::handleMessageReceived
    );

    connect(
        &gps_sensor, &GpsDeviceController::update_gps_data_signal,
        this, [&](const GpsUpdate& gps_update) {
            emit notifyEventPlaced(mapGpsUpdate(gps_update));
        }
    );
}

// TODO: make mappers reusable
const DeviceMessage AuxRecurrentEventMapper::mapGpsUpdate(
    const GpsUpdate& gps_update
) {
    auto bytes = json::to_msgpack(gps_update);

    return {
        "GPS",
        QByteArray(reinterpret_cast<const char*>(bytes.data()), bytes.size())
    };
};

