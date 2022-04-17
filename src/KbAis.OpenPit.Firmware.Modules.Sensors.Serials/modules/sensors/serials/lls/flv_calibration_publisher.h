#ifndef FLV_CALIBRATION_PUBLISHER_H
#define FLV_CALIBRATION_PUBLISHER_H

#include "core/configuration/app_configuration_manager.h"
#include "modules/sensors/serials/lls/lls_sensor_publisher.h"

using LlsCalibrationTable_t = std::vector<std::tuple<qint32, qint32>>;

struct flv_message {
    /*!
     * \brief Текущий уровень топлива в литрах, полученный при помощи
     * тарирования.
     */
    double fuel_level { 0.0 };

    /*!
     * \brief Максимальный уровень топлива в литрах.
     */
    double max_fuel_level { 0.0 };

    /*!
     * \brief Флаг указывающий, что текущий показатель уровня топлива является
     * действительным.
     */
    bool is_valid { false };

};

class flv_calibration_publisher {

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

    void start_publish_on(const rxcpp::observe_on_one_worker& coord);

    rxcpp::observable<flv_message> get_observable() const;

};

#endif // FLV_CALIBRATION_PUBLISHER_H
