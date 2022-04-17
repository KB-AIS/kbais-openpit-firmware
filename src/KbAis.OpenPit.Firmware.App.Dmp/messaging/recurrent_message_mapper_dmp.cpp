#include "recurrent_message_mapper_dmp.h"

// oss
#include <nlohmann/json.hpp>

// Core.Messaging
#include "Mappers/JsonMappers.h"
// Utils.Extensions
#include "QtExtensions/QByteArrayExt.h"

const QString MESSAGE_MONKIER_GPS { QStringLiteral("GPS") };

const QString MESSAGE_MONKIER_FUL { QStringLiteral("FUL") };

DmpRecurrentMessageMapper::DmpRecurrentMessageMapper(
    const i_gps_sensor_publisher& gps_message_pub
,   const flv_calibration_publisher& ful_message_pub
)
    :   m_gps_message_pub(gps_message_pub)
    ,   m_ful_message_pub(ful_message_pub)
{

}

rxcpp::observable<Message>
DmpRecurrentMessageMapper::getObservable() const {
    rxcpp::observable<Message> observable = rxcpp::observable<>::empty<Message>();

    rxcpp::observable<Message> gps_message_observable = m_gps_message_pub
        .get_observable()
        .map([&](const GpsMessage& x) -> Message {
            nlohmann::json j_object;
            j_object["datetime"] = x.isValid ? x.datetime : QDateTime::currentDateTimeUtc();
            j_object["isValid"] = x.isValid;
            j_object["latitude"] = x.latitude;
            j_object["longitude"] = x.longitude;
            j_object["speed"] = x.speed;

            return {
                MESSAGE_MONKIER_GPS
            ,   fromStdVector(nlohmann::json::to_msgpack(j_object))
            ,   QDateTime::currentDateTimeUtc(),
            };
        });

    rxcpp::observable<Message> ful_message_observable = m_ful_message_pub
        .get_observable()
        .map([&](const flv_message& x) -> Message {
            nlohmann::json j_object;
            j_object["FLV"] = x.fuel_level;
            j_object["VLD"] = x.is_valid;

            return {
                MESSAGE_MONKIER_FUL
            ,   fromStdVector(nlohmann::json::to_msgpack(j_object))
            ,   QDateTime::currentDateTimeUtc()
            };
        });

    return observable.merge(gps_message_observable, ful_message_observable);
}
