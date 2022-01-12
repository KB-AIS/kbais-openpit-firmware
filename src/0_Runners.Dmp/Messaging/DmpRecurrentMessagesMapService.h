#ifndef DMPRECURRENTMESSAGESMAPSERVICE_H
#define DMPRECURRENTMESSAGESMAPSERVICE_H

// qt
#include <QObject>

// cfw::infra::sensors::gps
#include "IRxGpsSensorPublisher.h"

#include "Messaging/Collectors/RecurrentMessagesCollector.h"
#include "Messaging/Message.h"

class DmpRecurrentMessagesMapService : public QObject {
    Q_OBJECT

public:
    DmpRecurrentMessagesMapService(
        RecurrentMessagesCollector& collector,
        const IRxGpsSensorPublisher& gpsSensorPublisher);

    Q_SIGNAL void messageMapped(const Message& message);

private:
    RecurrentMessagesCollector& collector;

    rxcpp::composite_subscription subs;

};

#endif // DMPRECURRENTMESSAGESMAPSERVICE_H
