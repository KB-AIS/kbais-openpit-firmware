#ifndef EVENTCOLLECTORSADAPTER_H
#define EVENTCOLLECTORSADAPTER_H

#include "recurrent_event_collector.h"
#include "immediate_event_collector.h"
// Qt
#include <QObject>
#include <QMutex>
// QDeferred
#include <QLambdaThreadWorker>

class EventCollectorsAdapter : public QObject {
    Q_OBJECT

public:
    explicit EventCollectorsAdapter(
        RecurrentEventCollector& recurrent_collector,
        ImmediateEventCollector& immediate_collector,
        QObject* parent = nullptr
    );

    Q_SLOT void handleImmediateEventPlaced();

private:
    void collectEventsFromAdapters();

    RecurrentEventCollector& recurrentCollector;

    ImmediateEventCollector& immediateCollector;

    QMutex collectEventsMtx;

    QLambdaThreadWorker worker;

};

#endif // EVENTCOLLECTORSADAPTER_H
