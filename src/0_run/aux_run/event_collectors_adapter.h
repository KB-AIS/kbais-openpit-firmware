#ifndef EVENTCOLLECTORSADAPTER_H
#define EVENTCOLLECTORSADAPTER_H

#include "recurrent_event_collector.h"
#include "immediate_event_collector.h"
// Qt
#include <QObject>
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

private:
    QLambdaThreadWorker m_worker;

};

#endif // EVENTCOLLECTORSADAPTER_H
