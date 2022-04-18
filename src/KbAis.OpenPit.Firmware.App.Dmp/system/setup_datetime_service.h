#ifndef SETUP_DATETIME_SERVICE_H
#define SETUP_DATETIME_SERVICE_H

#include "IRxGpsSensorPublisher.h"

/*!
 * \brief Сервис установки системного времени согласно показателям полученным
 * с GPS-устройства.
 */
class SetupDateTimeService {

    rxcpp::composite_subscription subscriptions_;

    void handle(const GpsMessage& msg);

public:
    SetupDateTimeService(const i_gps_sensor_publisher& gps_sensor_publisher);

    ~SetupDateTimeService() noexcept;

};

#endif // SETUP_DATETIME_SERVICE_H
