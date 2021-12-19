#include "aux_recurrent_messages_mapper.h"

// oss
#include <json.hpp>

using json = nlohmann::json;

using namespace KbAis::Cfw::Sensors::Gps;

// TODO: To mapping configuration file
namespace KbAis::Cfw::Sensors::Gps {

void to_json(json& j, const GpsUpdate& x) {
    j = json {
        { "datetime", x.datetime.toString().toStdString() },
        { "is_valid", x.isValid },
        { "latitude", x.latitude },
        { "longitude", x.longitude },
    };
}

void from_json(const json& j, GpsUpdate& x) {
    std::string dateTimeStr {};

    j.at("datetime").get_to(dateTimeStr);
    x.datetime = QDateTime::fromString(QString::fromStdString(dateTimeStr));
    j.at("is_valid").get_to(x.isValid);
    j.at("latitude").get_to(x.latitude);
    j.at("longitude").get_to(x.longitude);
}

}

AuxRecurrentEventMapper::AuxRecurrentEventMapper(
    const RecurrentMessagesCollector& collector,
    const SerialPortGpsDeviceController& gps_sensor,
    QObject* parent
): QObject(parent) {
    connect(
        this, &AuxRecurrentEventMapper::notifyEventPlaced,
        &collector, &RecurrentMessagesCollector::handleMessageReceived);

    connect(&gps_sensor, &BaseGpsDeviceController::notifyGpsDataUpdated, this, [&](auto x) {
        emit notifyEventPlaced(mapGpsUpdate(x));
    });
}

// TODO: make mappers reusable
const Message
AuxRecurrentEventMapper::mapGpsUpdate(
    const GpsUpdate& gpsUpdate
) {
    auto bytes = json::to_msgpack(gpsUpdate);

    return {
        "GPS",
        QByteArray(reinterpret_cast<const char*>(bytes.data()), bytes.size()),
        gpsUpdate.datetime
    };
};

