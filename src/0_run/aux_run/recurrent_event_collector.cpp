#include "recurrent_event_collector.h"

RecurrentEventCollector::RecurrentEventCollector(QObject *parent) : QObject(parent) { }

void
RecurrentEventCollector::handleEventPlaced(const Event& event) {
    QMutexLocker locker(&m_events_mutex);

    internalStore.insert(event.tag, event);
}

const QVector<Event>
RecurrentEventCollector::pop_events() {
    QMutexLocker locker(&m_events_mutex);

    QVector<Event> events { };
    for (auto& event : internalStore) {
        events.append(event);
    }

    internalStore.clear();

    return events;
}
