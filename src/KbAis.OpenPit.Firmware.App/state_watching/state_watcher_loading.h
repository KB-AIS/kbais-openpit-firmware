#ifndef STATEWATCHERLOADING_H
#define STATEWATCHERLOADING_H

#include "state_watching/state_watcher.h"
#include "state_watching/state_watcher_motioning.h"

#include "opf/modules/sensors/lcs/lcs_sensor_publisher.h"

struct state_watcher_loading_configuration {
    using duration_ms_t = std::chrono::duration<int64_t, std::milli>;

    double weight_threshold { 30.0 };

    double weight_threshold_dlt { 5.0 };

    double weight_avg_interval { 30.0 };

    duration_ms_t weight_fixation_duration { 10'000 };
};

class state_watcher_loading : public i_state_watcher_service, public i_state_loading_changed_publisher {
    using loading_state_t = rxcpp::rxsub::behavior<state_code_loading>;

    using time_point_t = std::chrono::steady_clock::time_point;

    struct impl_tk {
        double weight_filter { -1.0 }, weight_averege { 0.0 }, weight_fixed { 0.0 };

        bool has_weight_over_loading_threshold { false };

        bool is_weight_fixated { false };
        bool fix_last_time { false };
        time_point_t tp_before_fix_weight;

        state_code_motioning motioning_state;
    };

    const state_watcher_motioning& state_watcher_motioning_;

    const i_lcs_sensor_publisher& lcs_sensor_publisher_;

    loading_state_t current_loading_state_;

    impl_tk tk_;

    state_watcher_loading_configuration configuration;

    rxcpp::composite_subscription subscriptions_;

    void handle_weight_fixation(const double weight);

    void detect_loading_begin(const double weight);

    void detect_unloading_begin(const double weight);

    void detect_loading_finish();

    void detect_unloading_finish();

public:
    state_watcher_loading(
        const state_watcher_motioning& state_watcher_motioning
    ,   const i_lcs_sensor_publisher& lcs_sensor_publisher
    );

    void start_working_on(
        const rxcpp::observe_on_one_worker& coordination
    ) override;

    rxcpp::observable<state_code_loading> get_observable(
        const rxcpp::observe_on_one_worker &coordination
    ) const override;
};

#endif // STATEWATCHERLOADING_H
