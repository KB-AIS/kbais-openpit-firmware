#include "recurrent_message_mapper_dmp.h"

// oss
#include <nlohmann/json.hpp>

#include "QtExtensions/QByteArrayExt.h"
#include "qt_json.h"

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(gps_sensor_message
,   datetime
,   is_valid
,   latitude
,   longitude
,   speed
);

const QString MESSAGE_MONKIER_GPS { QStringLiteral("GPS") }
            , MESSAGE_MONKIER_FUL { QStringLiteral("FUL") };

DmpRecurrentMessageMapper::DmpRecurrentMessageMapper(
    const i_gps_sensor_publisher& gps_message_publisher
,   const i_flv_message_publisher& flv_message_publisher
)
    :   gps_message_publisher_(gps_message_publisher)
    ,   flv_message_publisher_(flv_message_publisher)
{
}

rxcpp::observable<Message>
DmpRecurrentMessageMapper::getObservable() const {
    rxcpp::observable<Message> observable = rxcpp::observable<>::empty<Message>();

    rxcpp::observable<Message> gps_message_observable = gps_message_publisher_
        .get_observable()
        .map([&](const gps_sensor_message& x) -> Message {
            nlohmann::json j_object;
            j_object["datetime"] = x.is_valid ? x.datetime : QDateTime::currentDateTimeUtc();
            j_object["is_valid"] = x.is_valid;
            j_object["latitude"] = x.latitude;
            j_object["longitude"] = x.longitude;
            j_object["speed"] = x.speed;

            return {
                MESSAGE_MONKIER_GPS
            ,   fromStdVector(nlohmann::json::to_msgpack(j_object))
            ,   QDateTime::currentDateTimeUtc(),
            };
        });

    rxcpp::observable<Message> ful_message_observable = flv_message_publisher_
        .get_observable()
        .map([&](const flv_message& x) -> Message {
            nlohmann::json j_object;
            j_object["fuel_level"] = x.fuel_level;
            j_object["is_valid"] = x.is_valid;

            return {
                MESSAGE_MONKIER_FUL
            ,   fromStdVector(nlohmann::json::to_msgpack(j_object))
            ,   QDateTime::currentDateTimeUtc()
            };
        });

    return observable.merge(gps_message_observable, ful_message_observable);
}
