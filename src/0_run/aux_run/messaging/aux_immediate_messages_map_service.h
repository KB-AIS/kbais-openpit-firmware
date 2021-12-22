#ifndef AUX_IMMEDIATE_MESSAGES_MAP_SERVICE_H
#define AUX_IMMEDIATE_MESSAGES_MAP_SERVICE_H

// qt
#include <QObject>

#include "host_wrapper.h"
#include "messaging/collectors/immediate_messages_collector.h"
#include "messaging/message.h"

class AuxImmediateMessagesMapService : public QObject {
    Q_OBJECT

public:
    AuxImmediateMessagesMapService(
        const ImmediateMessagesCollector& messagesCollector,
        const HostWrapper& host
    );

private:
    Q_SIGNAL void notifyIncommingMessageMapped(const Message& message);

};

#endif // AUX_IMMEDIATE_MESSAGES_MAP_SERVICE_H
