#include "event_collectors_adapter.h"

// Qt
#include <QThread>
#include <QDateTime>
// plog
#include <plog/Log.h>

EventCollectorsAdapter::EventCollectorsAdapter(
    RecurrentEventCollector& recurrentCollector,
    ImmediateEventCollector& immediateCollector,
    QObject* parent
) : QObject(parent),
    recurrentCollector(recurrentCollector),
    immediateCollector(immediateCollector) {

    connect(
        &immediateCollector, &ImmediateEventCollector::notifyImmediateEventPlaced,

        this, &EventCollectorsAdapter::handleImmediateEventPlaced
    );

    worker.startLoopInThread([&] { collectEventsFromAdapters(); }, 10 * 1000);
}

void
EventCollectorsAdapter::handleImmediateEventPlaced() {
    collectEventsFromAdapters();
}

void
EventCollectorsAdapter::collectEventsFromAdapters() {
    QMutexLocker lock(&collectEventsMtx);

    // TODO: Fix asyncron locking on both collectors, move locking control to outside
    PLOGD << "Collected at " << QDateTime::currentDateTime().toString();

    for (auto& event : recurrentCollector.pop_events()) {
        PLOGD << event.tag << " -- " << event.payload;
    }

    for (auto& event : immediateCollector.pop_events()) {
        PLOGD << event.tag << " -- " << event.payload;
    }
}
