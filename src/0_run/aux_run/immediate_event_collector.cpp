#include "immediate_event_collector.h"

ImmediateEventCollector::ImmediateEventCollector(QObject *parent) : QObject(parent) { }

void
ImmediateEventCollector::place_event_slot(const Event event)  {
    m_events.append(event);
}
