#include "immediate_event_collector.h"

// Qt
#include <QThread>
// plog
#include <plog/Log.h>

ImmediateEventCollector::ImmediateEventCollector(QObject *parent) : QObject(parent) {
    fireImmediateEventPlacedTimer.setSingleShot(true);

    connect(&fireImmediateEventPlacedTimer, &QTimer::timeout, this, [&] {
        emit notifyImmediateEventPlaced();
    });
}

void ImmediateEventCollector::handleEventPlaced(const Event event)  {
    QMutexLocker lock(&internalStoreMtx);

    internalStore.append(event);

    if (fireImmediateEventPlacedTimer.isActive()) {
        return;
    }

    // TODO: move timeout to const
    fireImmediateEventPlacedTimer.start(1000);
}

const QVector<Event> ImmediateEventCollector::pop_events() {
    QMutexLocker lock(&internalStoreMtx);

    QVector<Event> events;
    for (auto& event : internalStore) {
        events.append(event);
    }

    internalStore.clear();

    return events;
}
