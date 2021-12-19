#ifndef AUX_IMMEDIATE_MESSAGES_COLLECTOR_H
#define AUX_IMMEDIATE_MESSAGES_COLLECTOR_H

// qt
#include <QObject>

#include "device_message.h"
#include "host_wrapper.h"
#include "immediate_messages_collector.h"

class AuxImmediateMessagesMapper : public QObject {
    Q_OBJECT

public:
    AuxImmediateMessagesMapper(
        const ImmediateMessagesCollector& collector,
        const HostWrapper* host,
        QObject *parent = nullptr
    );

private:
    Q_SIGNAL void notifyMessageReceived(const Message& msg);

};

#endif // AUX_IMMEDIATE_MESSAGES_COLLECTOR_H
