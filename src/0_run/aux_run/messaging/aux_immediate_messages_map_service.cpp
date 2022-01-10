#include "aux_immediate_messages_map_service.h"

// oss
#include <nlohmann/json.hpp>

using json = nlohmann::json;

AuxImmediateMessagesMapService::AuxImmediateMessagesMapService(
    const ImmediateMessagesCollector& messagesCollector,
    const HostWrapper& host
) : QObject() {
    connect(
        this, &AuxImmediateMessagesMapService::notifyIncommingMessageMapped,

        &messagesCollector, &ImmediateMessagesCollector::handleMessagePlaced
    );

    connect(
        host.mainPresenter, &main_presenter::notifyTestUserEvent,

        this, [&] {
            const auto payload = json::to_msgpack("Some user's UI event");

            emit notifyIncommingMessageMapped({
                "MSG",
                QByteArray(reinterpret_cast<const char*>(payload.data()), payload.size()),
                QDateTime::currentDateTimeUtc()
            });
        }
    );
}
