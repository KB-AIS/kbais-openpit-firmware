#ifndef RECURRENT_MESSAGE_MAPPER_DMP_H
#define RECURRENT_MESSAGE_MAPPER_DMP_H

#include "IRxGpsSensorPublisher.h"
#include "IRxRecurrentMessageMapper.h"
#include "Message.h"
#include "modules/sensors/serials/lls/flv_calibration_publisher.h"

class DmpRecurrentMessageMapper : public IRxRecurrentMessageMapper {
    const i_gps_sensor_publisher& m_gps_message_pub;

    const flv_calibration_publisher& m_ful_message_pub;

public:
    DmpRecurrentMessageMapper(
        const i_gps_sensor_publisher& gps_message_pub
    ,   const flv_calibration_publisher& ful_message_pub
    );

    rxcpp::observable<Message> getObservable() const override;

};

#endif // RECURRENT_MESSAGE_MAPPER_DMP_H
