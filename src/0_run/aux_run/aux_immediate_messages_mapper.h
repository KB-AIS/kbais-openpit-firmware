#ifndef AUXIMMEDIATEMESSAGESCOLLECTOR_H
#define AUXIMMEDIATEMESSAGESCOLLECTOR_H

#include "device_message.h"
#include "host_wrapper.h"
#include "immediate_messages_collector.h"
// Qt
#include <QObject>

class AuxImmediateMessagesMapper : public QObject {
    Q_OBJECT

public:
    explicit AuxImmediateMessagesMapper(
        const ImmediateMessagesCollector& collector,
        const HostWrapper* host,
        QObject *parent = nullptr
    );

private:
    Q_SIGNAL void notifyMessageReceived(const DeviceMessage& msg);

};

#endif // AUXIMMEDIATEMESSAGESCOLLECTOR_H
