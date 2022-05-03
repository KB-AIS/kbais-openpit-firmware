#ifndef FLV_CALIBRATION_PUBLISHER_H
#define FLV_CALIBRATION_PUBLISHER_H

#include "core/configuration/app_configuration_manager.h"

#include "opf/modules/sensors/flv/flv_message_publisher.h"
#include "opf/modules/sensors/lls/lls_sensor_publisher.h"

using LlsCalibrationTable_t = std::vector<std::tuple<qint32, qint32>>;

class flv_calibration_publisher : public i_flv_message_publisher {

    const i_app_configuration_publisher& app_configuration_publisher_;

    const lls_sensor_publisher& lls_sensor_publisher_;

    rxcpp::composite_subscription subscriptions_;

    rxcpp::rxsub::behavior<flv_message> last_flv_message_;

    LlsCalibrationTable_t calibration_table_;

    double max_fuel_level_ { 0.0 };

    void handle_new_scale_config(const app_configuration& conf);

    void handle_fuel_calibration(const lls_sensor_message& msg) const;

    double get_fuel_level_by_calibration_table(double lls_value) const;

public:
    flv_calibration_publisher(
        i_app_configuration_publisher& app_configuration_publisher
    ,   lls_sensor_publisher& lls_sensor_publisher
    );

    void start_publish_on(const rxcpp::observe_on_one_worker& c);

    rxcpp::observable<flv_message> get_observable(const rxcpp::observe_on_one_worker& c) const override;

};

#endif // FLV_CALIBRATION_PUBLISHER_H
