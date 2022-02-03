#include "DmpRecurrentMessageMapper.h"

// oss
#include <nlohmann/json.hpp>

// Core.Messaging
#include "Mappers/JsonMappers.h"
// Utils.Extensions
#include "QtExtensions/QByteArrayExt.h"

const QString MESSAGE_MONKIER_GPS { QStringLiteral("GPS") };

DmpRecurrentMessageMapper::DmpRecurrentMessageMapper(
    const IRxGpsSensorPublisher& gpsSensorPublisher
)
    :   mGpsSensorPublisher { gpsSensorPublisher }
{

}

rxcpp::observable<Message>
DmpRecurrentMessageMapper::getObservable() const {
    auto const gpsMessageObservable = mGpsSensorPublisher.getObservable()
        .map([&](const GpsMessage& x) -> Message {
            return {
                MESSAGE_MONKIER_GPS,
                fromStdVector(nlohmann::json::to_msgpack(x)),
                QDateTime::currentDateTimeUtc(),
            };
        });

    return gpsMessageObservable;
}
