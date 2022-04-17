#include "state_watcher_loading.h"

// oss
#include <plog/Log.h>
#include <fmt/format.h>

using namespace std::chrono_literals;
using namespace rx;

constexpr auto TONS_TO_10KILOGRAMMS_MUL = 100.0;

state_watcher_loading::state_watcher_loading(
    const state_watcher_motioning& state_watcher_motioning
,   const i_lcs_sensor_publisher& lcs_sensor_publisher
)
    :   state_watcher_motioning_ { state_watcher_motioning }
    ,   lcs_sensor_publisher_ { lcs_sensor_publisher }
    ,   current_loading_state_ { state_code_loading::empty }
{
}

void
state_watcher_loading::start_working_on(const rxcpp::observe_on_one_worker& coordination) {
    auto obs_motioning_state = state_watcher_motioning_.get_observable(coordination);

    auto obs_lcs_sensor_message = lcs_sensor_publisher_.get_observable(coordination)
    |   filter([](const lcs_sensor_message& x) -> bool {
            return x.is_valid;
        });

    auto obs_one_second_passed = rxcpp::observable<>::interval(1s, coordination);

    obs_motioning_state | subscribe<state_code_motioning>(
        subscriptions_
    ,   [this](const state_code_motioning& x) { tk_.motioning_state = x; }
    );

    obs_lcs_sensor_message | subscribe<lcs_sensor_message>(
        subscriptions_
    ,   [this](const lcs_sensor_message& x) { handle_weight_fixation(x.weight); }
    );

    obs_lcs_sensor_message | subscribe<lcs_sensor_message>(
        subscriptions_
    ,   [this](const lcs_sensor_message& x) { detect_loading_begin(x.weight); }
    );

    obs_lcs_sensor_message | subscribe<lcs_sensor_message>(
        subscriptions_
    ,   [this](const lcs_sensor_message& x) { detect_unloading_begin(x.weight); }
    );

    obs_one_second_passed | subscribe<long>(
        subscriptions_
    ,   [this](auto) { detect_loading_finish(); }
    );

    obs_one_second_passed | subscribe<long>(
        subscriptions_
    ,   [this](int) { detect_unloading_finish(); }
    );
}

rxcpp::observable<state_code_loading> state_watcher_loading::get_observable(
    const rxcpp::observe_on_one_worker& coordination
) const {
    return current_loading_state_.get_observable().observe_on(coordination);
}

void state_watcher_loading::handle_weight_fixation(const double weight) {
    if (tk_.is_weight_fixated) {
        tk_.weight_averege = tk_.weight_fixed;
        return;
    }

    if (tk_.motioning_state != state_code_motioning::moving) {
        tk_.weight_filter = -1.0;
        tk_.weight_averege = weight;
        return;
    }

    if ((configuration.weight_avg_interval > 1) && (tk_.weight_filter > 0.0)) {
        const auto interval = configuration.weight_avg_interval;
        tk_.weight_filter = tk_.weight_filter * ((interval - 1.0) / interval) + weight / interval;
    } else {
        tk_.weight_filter = weight;
    }

    tk_.weight_averege = tk_.weight_filter;
}

void state_watcher_loading::detect_loading_begin(const double weight) {
    const auto weight_threshold_10kgs =
        (configuration.weight_threshold + configuration.weight_threshold_dlt) * TONS_TO_10KILOGRAMMS_MUL;

    const auto need_to_begin_loading = weight > weight_threshold_10kgs && !tk_.has_weight_over_loading_threshold;
    if (!need_to_begin_loading) return;

    tk_.has_weight_over_loading_threshold = true;

    const auto is_moving_state = tk_.motioning_state == state_code_motioning::moving;
    if (is_moving_state) return;

    current_loading_state_.get_subscriber().on_next(state_code_loading::loading);
}

void state_watcher_loading::detect_unloading_begin(const double weight_10kg) {
    const auto weight_threshold_10kgs =
        (configuration.weight_threshold - configuration.weight_threshold_dlt) * TONS_TO_10KILOGRAMMS_MUL;

    const auto need_to_begin_unloading = weight_10kg < weight_threshold_10kgs;
    if (!need_to_begin_unloading) return;

    if (tk_.has_weight_over_loading_threshold) {
        current_loading_state_.get_subscriber().on_next(state_code_loading::unloading);
    }

    tk_.has_weight_over_loading_threshold = false;
}

void state_watcher_loading::detect_loading_finish() {
    if (!tk_.has_weight_over_loading_threshold) return;

    const auto tp_now = std::chrono::steady_clock::now();

    const auto need_start_weight_fixation =
        !tk_.is_weight_fixated
    &&  !tk_.fix_last_time
    &&  tk_.motioning_state == state_code_motioning::moving;

    if (need_start_weight_fixation) {
        tk_.fix_last_time = true;
        tk_.tp_before_fix_weight = tp_now;
    }

    const auto need_to_finish_loading =
        tk_.fix_last_time
    &&  (tp_now - tk_.tp_before_fix_weight > configuration.weight_fixation_duration);
    if (!need_to_finish_loading) return;

    tk_.is_weight_fixated = true;
    tk_.fix_last_time = false;
    tk_.weight_fixed = tk_.weight_averege;

    current_loading_state_.get_subscriber().on_next(state_code_loading::loaded);
}

void state_watcher_loading::detect_unloading_finish() {
    if (tk_.has_weight_over_loading_threshold) return;

    tk_.is_weight_fixated = false;
    tk_.fix_last_time = false;

    const auto need_to_finish_unloading =
        current_loading_state_.get_value() == state_code_loading::unloading
    &&  tk_.motioning_state == state_code_motioning::moving;
    if (!need_to_finish_unloading) return;

    current_loading_state_.get_subscriber().on_next(state_code_loading::empty);

    tk_.weight_fixed = 0.0;
}
