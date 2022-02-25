#include "DmpRecurrentMessageMapper.h"

// oss
#include <nlohmann/json.hpp>

// Core.Messaging
#include "Mappers/JsonMappers.h"
// Utils.Extensions
#include "QtExtensions/QByteArrayExt.h"

const QString MESSAGE_MONKIER_GPS { QStringLiteral("GPS") };

const QString MESSAGE_MONKIER_FUL { QStringLiteral("FUL") };

DmpRecurrentMessageMapper::DmpRecurrentMessageMapper(
    const IRxGpsSensorPublisher& gps_message_pub
,   const RxFuelMessagePublisher& ful_message_pub
)
    :   m_gps_message_pub(gps_message_pub)
    ,   m_ful_message_pub(ful_message_pub)
{

}

rxcpp::observable<Message>
DmpRecurrentMessageMapper::getObservable() const {
    rxcpp::observable<Message> observable =
        rxcpp::observable<>::empty<Message>();

    rxcpp::observable<Message> gps_message_observable = m_gps_message_pub
        .GetObservable()
        .map([&](const GpsMessage& x) -> Message {
            return {
                MESSAGE_MONKIER_GPS
            ,   fromStdVector(nlohmann::json::to_msgpack(x))
            ,   QDateTime::currentDateTimeUtc(),
            };
        });

    rxcpp::observable<Message> ful_message_observable = m_ful_message_pub
        .get_obeservable_fuel_message()
        .map([&](const FuelMessage& x) -> Message {
            nlohmann::json j_object;
            j_object["FLV"] = x.cur_fuel_level;
            j_object["VLD"] = x.is_value_valid;

            return {
                MESSAGE_MONKIER_FUL
            ,   fromStdVector(nlohmann::json::to_msgpack(j_object))
            ,   QDateTime::currentDateTimeUtc()
            };
        });

    return observable.merge(gps_message_observable, ful_message_observable);
}
