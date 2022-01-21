#include "DmpRecurrentMessagesMapService.h"

// cfw::trdpary
#include "RxQt/RxQt.h"

// cfw::utils
#include "Extensions/QByteArrayExt.h"

#include "Messaging/Mappers/JsonMappers.h"

const QString MESSAGE_MONKIER_GPS { QStringLiteral("GPS") };

DmpRecurrentMessagesMapService::DmpRecurrentMessagesMapService(
    const IRxGpsSensorPublisher& gpsSensorPublisher
)
    : mGpsSensorPublisher { gpsSensorPublisher }
{

}

rxcpp::observable<Message>
DmpRecurrentMessagesMapService::getMessageObservable() const {
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
