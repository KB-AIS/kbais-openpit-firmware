#ifndef DMPRECURRENTMESSAGEMAPPER_H
#define DMPRECURRENTMESSAGEMAPPER_H

#include "IRxGpsSensorPublisher.h"
#include "IRxRecurrentMessageMapper.h"
#include "Message.h"
#include "Modules/Sensors/Serials/RxFuelMessagePublisher.h"

class DmpRecurrentMessageMapper : public IRxRecurrentMessageMapper {
    const IRxGpsSensorPublisher& m_gps_message_pub;

    const RxFuelMessagePublisher& m_ful_message_pub;

public:
    DmpRecurrentMessageMapper(
        const IRxGpsSensorPublisher& gps_message_pub
    ,   const RxFuelMessagePublisher& ful_message_pub
    );

    rxcpp::observable<Message> getObservable() const override;

};

#endif // DMPRECURRENTMESSAGEMAPPER_H
