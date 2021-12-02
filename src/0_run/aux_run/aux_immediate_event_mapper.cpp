#include "aux_immediate_event_mapper.h"

AuxImmediateEventMapper::AuxImmediateEventMapper(
    const ImmediateEventCollector& immediateEventCollector,
    // TODO: Provide access point to UI events
    const HostWrapper* host,
    QObject *parent
) : QObject(parent) {
    connect(
        this, &AuxImmediateEventMapper::notifyEventPlaced,
        &immediateEventCollector, &ImmediateEventCollector::handleEventPlaced,
        Qt::ConnectionType::QueuedConnection
    );

    connect(
        host->m_main_presenter, &main_presenter::notifyTestUserEvent,
        this, [&] {
            emit notifyEventPlaced({ "TEST", "Some user's UI event" });
        }
    );
}
