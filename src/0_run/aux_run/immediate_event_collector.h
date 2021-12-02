#ifndef IMMEDIATEEVENTCOLLECTOR_H
#define IMMEDIATEEVENTCOLLECTOR_H

#include "event.h"
// Qt
#include <QMutex>
#include <QObject>
#include <QTimer>
#include <QVector>

class ImmediateEventCollector : public QObject {
    Q_OBJECT

public:
    explicit ImmediateEventCollector(QObject *parent = nullptr);

    Q_SLOT void handleEventPlaced(const Event event);

    const QVector<Event> pop_events();

    Q_SIGNAL void notifyImmediateEventPlaced();

private:
    QList<Event> internalStore;

    QMutex internalStoreMtx;

    QTimer fireImmediateEventPlacedTimer;

};

#endif // IMMEDIATEEVENTCOLLECTOR_H
