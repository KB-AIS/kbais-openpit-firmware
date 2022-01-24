#ifndef DMPRECURRENTMESSAGESMAPSERVICE_H
#define DMPRECURRENTMESSAGESMAPSERVICE_H

// cfw::infra::sensors::gps
#include "IRxGpsSensorPublisher.h"

#include "Messaging/Message.h"

class DmpRecurrentMessagesMapService {

public:
    DmpRecurrentMessagesMapService(const IRxGpsSensorPublisher& gpsSensorPublisher);

    rxcpp::observable<Message> getMessageObservable() const;

private:
    const IRxGpsSensorPublisher& mGpsSensorPublisher;

};

#endif // DMPRECURRENTMESSAGESMAPSERVICE_H
