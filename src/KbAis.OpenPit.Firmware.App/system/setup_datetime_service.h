#ifndef SETUP_DATETIME_SERVICE_H
#define SETUP_DATETIME_SERVICE_H

#include "IRxGpsSensorPublisher.h"

/*!
 * \brief Сервис установки системного времени согласно показателям полученным
 * с GPS-устройства.
 */
class setup_datetime_service {

    rxcpp::composite_subscription subscriptions_;

    static void handle_gps_message(const GpsMessage& msg);

    static bool need_sync_system_datetime(
        const QDateTime& gps_datetime, const QDateTime& sys_datetime
    );

    static void sync_system_datetime(const QDateTime& datetime);

public:
    setup_datetime_service(const i_gps_sensor_publisher& gps_sensor_publisher);

    ~setup_datetime_service() noexcept;

};

#endif // SETUP_DATETIME_SERVICE_H
