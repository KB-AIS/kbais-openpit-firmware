#ifndef DMPRECURRENTMESSAGEMAPPER_H
#define DMPRECURRENTMESSAGEMAPPER_H

// Core.Messaging
#include "IRxRecurrentMessageMapper.h"
#include "Message.h"
// Modules.Sensors.Gps
#include "IRxGpsSensorPublisher.h"

class DmpRecurrentMessageMapper : public IRxRecurrentMessageMapper {

public:
    DmpRecurrentMessageMapper(const IRxGpsSensorPublisher& gpsSensorPublisher);

    rxcpp::observable<Message> getObservable() const override;

private:
    const IRxGpsSensorPublisher& mGpsSensorPublisher;

};

#endif // DMPRECURRENTMESSAGEMAPPER_H
