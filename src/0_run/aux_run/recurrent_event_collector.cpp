#include "recurrent_event_collector.h"

RecurrentEventCollector::RecurrentEventCollector(QObject *parent) : QObject(parent) { }

void
RecurrentEventCollector::place_event_slot(const Event& event) {
    QMutexLocker locker(&m_events_mutex);

    m_events.insert(event.m_tag, event);
}

const QList<Event>
RecurrentEventCollector::pop_events() {
    QMutexLocker locker(&m_events_mutex);

    auto const events = m_events.values();
    m_events.clear();

    return events;
}
