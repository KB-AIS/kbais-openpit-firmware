#include "DmpImmediateMessagesMapService.h"

// cfw::utils
#include "Extensions/QByteArrayExt.h"

#include "Messaging/Mappers/JsonMappers.h"

const QString MESSAGE_MONKIER_USR { QStringLiteral("USR") };

DmpImmediateMessagesMapService::DmpImmediateMessagesMapService(
    ImmediateMessagesCollector& collector
)
    : QObject()
{
    subs = rxcpp::composite_subscription();

//    connect(
//        host.mainPresenter, &main_presenter::notifyTestUserEvent,
//        this, [&] {
//            emit messageMapped({
//                MESSAGE_MONKIER_USR,
//                fromStdVector(nlohmann::json::to_msgpack("Some user's UI event")),
//                QDateTime::currentDateTimeUtc()
//            });
//        }
//    );

    rxqt::from_signal(this, &DmpImmediateMessagesMapService::messageMapped)
        .subscribe(subs, [&](const Message& x) { collector.placeMessage(x); });
}
