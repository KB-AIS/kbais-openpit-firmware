#include "aux_immediate_messages_mapper.h"

// oss
#include <json.hpp>

using json = nlohmann::json;

AuxImmediateMessagesMapper::AuxImmediateMessagesMapper(
    const ImmediateMessagesCollector& immediateEventCollector,
    // TODO: Provide access point to UI events
    const HostWrapper* host,
    QObject *parent
) : QObject(parent) {
    connect(
        this, &AuxImmediateMessagesMapper::notifyMessageReceived,

        &immediateEventCollector, &ImmediateMessagesCollector::handleMessageReceived
    );

    connect(
        host->mainPresenter, &main_presenter::notifyTestUserEvent,
        this, [&] {
            auto bytes = json::to_msgpack("Some user's UI event");

            emit notifyMessageReceived({
                "TEST",
                QByteArray(reinterpret_cast<const char*>(bytes.data()), bytes.size())
            });
        }
    );
}
