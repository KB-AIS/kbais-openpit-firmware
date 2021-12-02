#ifndef RECURRENTEVENTCOLLECTOR_H
#define RECURRENTEVENTCOLLECTOR_H

#include "event.h"
// Qt
#include <QHash>
#include <QMutex>
#include <QObject>
#include <QVector>

class RecurrentEventCollector : public QObject {
    Q_OBJECT

public:
    explicit RecurrentEventCollector(QObject *parent = nullptr);

    Q_SLOT void handleEventPlaced(const Event& event);

    const QVector<Event> pop_events();

private:
    QHash<QString, Event> internalStore;

    QMutex m_events_mutex;

};

#endif // RECURRENTEVENTCOLLECTOR_H
