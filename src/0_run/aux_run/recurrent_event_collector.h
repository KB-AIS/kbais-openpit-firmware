#ifndef RECURRENTEVENTCOLLECTOR_H
#define RECURRENTEVENTCOLLECTOR_H

#include "event.h"
// Qt
#include <QHash>
#include <QMutex>
#include <QObject>

class RecurrentEventCollector : public QObject {
    Q_OBJECT

public:
    explicit RecurrentEventCollector(QObject *parent = nullptr);

    Q_SLOT void place_event_slot(const Event& event);

    const QList<Event> pop_events();

private:
    QHash<QString, Event> m_events;

    QMutex m_events_mutex;

};

#endif // RECURRENTEVENTCOLLECTOR_H
