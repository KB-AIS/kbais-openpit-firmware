#ifndef STATE_WATCHER_FUELLING_H
#define STATE_WATCHER_FUELLING_H

// std
#include <chrono>

#include "modules/sensors/serials/lls/flv_calibration_publisher.h"
#include "state_watching/state_watcher_motioning.h"

class StateWatcherFuelling : public i_state_watcher_service {
    using SteadyClock_t = std::chrono::steady_clock::time_point;

    flv_calibration_publisher& ful_msg_pub_;

    rxcpp::rxsub::behavior<state_changed_message> subject_state_message_;

    std::vector<double> m_fls;
    int                 m_fl_current_idx { 0 };
    double              m_fl_filtered { 0.0 };

    bool                m_fl_fixed { false };
    SteadyClock_t       m_fl_last_unfixed_time;
    double              m_fl_fixed_tmp { 0.0 };

    bool                m_state_fueling { false };
    double              m_fl_fueling_begin { 0.0 };

    bool                m_state_draining { false };
    double              m_fl_draining_begin { 0.0 };

    void handle_fuel_message(const flv_message& fuel_message);

    void publish_new_state(state_code state_code);

public:
    StateWatcherFuelling(flv_calibration_publisher& ful_msg_pub);

    void start_working_on(const rxcpp::observe_on_one_worker& coordination) override;

    rxcpp::observable<state_changed_message> get_observable() const;

};

#endif // STATE_WATCHER_FUELLING_H
