#include "event_collectors_adapter.h"

// Qt
#include <QThread>
// plog
#include <plog/Log.h>

EventCollectorsAdapter::EventCollectorsAdapter(
    RecurrentEventCollector& recurrent_collector,
    ImmediateEventCollector& /*immediate_collector*/,
    QObject* parent
) : QObject(parent) {

    m_worker.startLoopInThread([&recurrent_collector]() {
        PLOGD << "Do work in thead: " << QThread::currentThread();
        for (auto& event : recurrent_collector.pop_events()) {
            PLOGD << event.m_tag << " -- " << event.m_payload;
        }
    }, 10 * 1000 /* every 10 seconds */);

}
