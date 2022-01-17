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
        const IRxGpsSensorPublisher& gpsSensorPublisher
    );

    ~DmpRecurrentMessagesMapService();

private:
    rxcpp::composite_subscription subs;

    Q_SIGNAL void messageMapped(const Message& message);

};

#endif // DMPRECURRENTMESSAGESMAPSERVICE_H
