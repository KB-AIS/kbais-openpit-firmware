#include "DmpRecurrentMessagesMapService.h"

// cfw::trdpary
#include "RxQt/RxQt.h"

// cfw::utils
#include "Extensions/QByteArrayExt.h"

#include "Messaging/Mappers/JsonMappers.h"

const QString MESSAGE_MONKIER_GPS { QStringLiteral("GPS") };

DmpRecurrentMessagesMapService::DmpRecurrentMessagesMapService(
    RecurrentMessagesCollector& collector,
    const IRxGpsSensorPublisher& gpsSensorPublisher
)
    : QObject()
{
    subs = rxcpp::composite_subscription();

    gpsSensorPublisher.getObservable()
        .subscribe(subs, [&](const GpsUpdateDto& gpsUpdate) {
            Message message {
                MESSAGE_MONKIER_GPS,
                fromStdVector(nlohmann::json::to_msgpack(gpsUpdate)),
                QDateTime::currentDateTimeUtc(),
            };

            emit messageMapped(message);
        });

    rxqt::from_signal(this, &DmpRecurrentMessagesMapService::messageMapped)
        .subscribe(subs, [&](const Message& message) {
            collector.placeMessage(message);
        });
}

DmpRecurrentMessagesMapService::~DmpRecurrentMessagesMapService() {
    subs.unsubscribe();
}
