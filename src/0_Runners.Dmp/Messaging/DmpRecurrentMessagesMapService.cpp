#include "DmpRecurrentMessagesMapService.h"

// trdpary::rxqt
#include "RxQt.h"

#include "Messaging/Mappers/JsonMappers.h"

const QString MESSAGE_MONKIER_GPS_UPDATE { "GPS" };

DmpRecurrentMessagesMapService::DmpRecurrentMessagesMapService(
    RecurrentMessagesCollector& collector,
    const IRxGpsSensorPublisher& gpsSensorPublisher)
    : QObject()
    , collector { collector }
{
    subs = rxcpp::composite_subscription();

    gpsSensorPublisher.getObservable()
        .subscribe(subs, [&](const GpsUpdateDto& gpsUpdate) {
            const auto payload = nlohmann::json::to_msgpack(gpsUpdate);

            Message message {
                MESSAGE_MONKIER_GPS_UPDATE,
                QByteArray(reinterpret_cast<const char*>(payload.data()), payload.size()),
                QDateTime::currentDateTimeUtc(),
            };

            emit messageMapped(message);
        });

    rxqt::from_signal(this, &DmpRecurrentMessagesMapService::messageMapped)
        .subscribe(subs, [&](const Message& message) {
            this->collector.storeMessage(message);
        });
}
