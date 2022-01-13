#ifndef DMPIMMEDIATEMESSAGESMAPSERVICE_H
#define DMPIMMEDIATEMESSAGESMAPSERVICE_H

// qt
#include <QObject>

// #include "host_wrapper.h"
#include "Messaging/Collectors/ImmediateMessagesCollector.h"
#include "Messaging/Message.h"

class DmpImmediateMessagesMapService : public QObject {
    Q_OBJECT

public:
    DmpImmediateMessagesMapService(
        const ImmediateMessagesCollector& messagesCollector/*,
        const HostWrapper& host*/
    );

private:
    Q_SIGNAL void notifyIncommingMessageMapped(const Message& message);

    QTimer* timer;

};

#endif // DMPIMMEDIATEMESSAGESMAPSERVICE_H
