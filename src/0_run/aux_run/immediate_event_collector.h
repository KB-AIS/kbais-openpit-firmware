#ifndef IMMEDIATEEVENTCOLLECTOR_H
#define IMMEDIATEEVENTCOLLECTOR_H

#include "event.h"
// Qt
#include <QList>
#include <QObject>

class ImmediateEventCollector : public QObject {
    Q_OBJECT

public:
    explicit ImmediateEventCollector(QObject *parent = nullptr);

    Q_SLOT void place_event_slot(const Event event);

private:
    QList<Event> m_events;

};

#endif // IMMEDIATEEVENTCOLLECTOR_H
