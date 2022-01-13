#include "DmpImmediateMessagesMapService.h"

// cfw::utils
#include "Extensions/QByteArrayExt.h"

#include "Messaging/Mappers/JsonMappers.h"

DmpImmediateMessagesMapService::DmpImmediateMessagesMapService(
    const ImmediateMessagesCollector& messagesCollector/*,
    const HostWrapper& host*/
)
    : QObject()
    , timer { new QTimer(this) }
{
    connect(
        this, &DmpImmediateMessagesMapService::notifyIncommingMessageMapped,
        &messagesCollector, &ImmediateMessagesCollector::handleMessagePlaced
    );

    timer->setInterval(3000);

    connect(
        //host.mainPresenter, &main_presenter::notifyTestUserEvent,
        timer, &QTimer::timeout,
        this, [&] {
            emit notifyIncommingMessageMapped({
                "MSG",
                fromStdVector(nlohmann::json::to_msgpack("Some user's UI event")),
                QDateTime::currentDateTimeUtc()
            });
        }
    );

    timer->start();
}
