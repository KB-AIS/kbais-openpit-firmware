#ifndef FAKE_LCS_SENSOR_PUBLISHER_H
#define FAKE_LCS_SENSOR_PUBLISHER_H

#include "modules/sensors/serials/lcs/lcs_sensor_publisher.h"
#include "simulating/scenario_executor.h"

class fake_lcs_sensor_publisher : public i_lcs_sensor_publisher, public i_scenario_executor {
    rxcpp::connectable_observable<lcs_sensor_message> obs_lcs_sensor_messages_;
    rxcpp::rxsub::subject<lcs_sensor_message>         sub_lcs_sensor_messages_;

public:
    fake_lcs_sensor_publisher() noexcept;

    rxcpp::observable<lcs_sensor_message> get_observable(
        const rxcpp::observe_on_one_worker& coordination
    ) const override;

    void setup_scenario(const rxcpp::observe_on_one_worker& coordination) override;

    void start_scenario() override;
};

#endif // FAKE_LCS_SENSOR_PUBLISHER_H
