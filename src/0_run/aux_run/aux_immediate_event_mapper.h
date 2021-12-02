#ifndef AUXIMMEDIATEEVENTCOLLECTOR_H
#define AUXIMMEDIATEEVENTCOLLECTOR_H

#include "event.h"
#include "host_wrapper.h"
#include "immediate_event_collector.h"
// Qt
#include <QObject>

class AuxImmediateEventMapper : public QObject {
    Q_OBJECT

public:
    explicit AuxImmediateEventMapper(
        const ImmediateEventCollector& immediateEventCollector,
        const HostWrapper* host,
        QObject *parent = nullptr
    );

private:
    Q_SIGNAL void notifyEventPlaced(const Event& event);

};

#endif // AUXIMMEDIATEEVENTCOLLECTOR_H
