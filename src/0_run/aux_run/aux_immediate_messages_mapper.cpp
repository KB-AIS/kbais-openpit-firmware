#include "aux_immediate_messages_mapper.h"

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
        host->m_main_presenter, &main_presenter::notifyTestUserEvent,
        this, [&] {
            emit notifyMessageReceived({
                "TEST",
                "Some user's UI event"
            });
        }
    );
}
