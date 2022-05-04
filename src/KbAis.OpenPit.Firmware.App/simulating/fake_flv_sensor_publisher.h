#ifndef FAKE_FLV_SENSOR_PUBLISHER_H
#define FAKE_FLV_SENSOR_PUBLISHER_H

#include "simulating/scenario_executor.h"

#include "opf/modules/sensors/flv/flv_message_publisher.h"

class fake_flv_sensor_publisher : public i_flv_message_publisher, public i_scenario_executor {
    rxcpp::connectable_observable<flv_message> obs_flv_messages_;
    rxcpp::rxsub::behavior<flv_message>        sub_flv_messages_;

public:
    fake_flv_sensor_publisher() noexcept;

    rxcpp::observable<flv_message> get_observable() const;

    void setup_scenario(const rxcpp::observe_on_one_worker& c) override;

    void start_scenario() override;

};

#endif // FAKE_FLV_SENSOR_PUBLISHER_H
