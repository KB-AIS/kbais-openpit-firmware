#ifndef DMPIMMEDIATEMESSAGESMAPSERVICE_H
#define DMPIMMEDIATEMESSAGESMAPSERVICE_H

// qt
#include <QObject>

// cfw::trdpary
#include "RxQt/RxQt.h"

#include "Messaging/Collectors/ImmediateMessagesCollector.h"
#include "Messaging/Message.h"

class DmpImmediateMessagesMapService : public QObject {
    Q_OBJECT

public:
    DmpImmediateMessagesMapService(
        ImmediateMessagesCollector& messagesCollector
    );

private:
    rxcpp::composite_subscription subs;

    Q_SIGNAL void messageMapped(const Message& message);
};

#endif // DMPIMMEDIATEMESSAGESMAPSERVICE_H
