#ifndef RECURRENT_MESSAGE_MAPPER_DMP_H
#define RECURRENT_MESSAGE_MAPPER_DMP_H

#include "IRxRecurrentMessageMapper.h"
#include "Message.h"

#include "opf/modules/sensors/flv/flv_message_publisher.h"
#include "opf/modules/sensors/gps/gps_sensor_publisher.h"

class DmpRecurrentMessageMapper : public IRxRecurrentMessageMapper {
    const i_gps_sensor_publisher& gps_message_publisher_;

    const i_flv_message_publisher& flv_message_publisher_;

public:
    DmpRecurrentMessageMapper(
        const i_gps_sensor_publisher& gps_message_publisher
    ,   const i_flv_message_publisher& flv_message_publisher
    );

    rxcpp::observable<Message> getObservable() const override;

};

#endif // RECURRENT_MESSAGE_MAPPER_DMP_H
