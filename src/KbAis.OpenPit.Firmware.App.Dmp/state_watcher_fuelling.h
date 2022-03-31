#ifndef STATE_WATCHER_FUELLING_H
#define STATE_WATCHER_FUELLING_H

// std
#include <chrono>

#include "Modules/Sensors/Serials/RxFuelMessagePublisher.h"

enum class StateCode {
    Unknown
,   Move
,   Stop
,   Park
,   LoadBegin
,   LoadFinish
,   UnloadBegin
,   UnloadFinish
,   RefuelBegin
,   RefuelFinish
,   DrainBegin
,   DrainFinish
};

struct StateMessage {
    StateCode code;
};

class StateWatcherFuelling {
    using SteadyClock_t = std::chrono::steady_clock::time_point;

    RxFuelMessagePublisher& ful_msg_pub_;

    rxcpp::rxsub::behavior<StateMessage> subject_state_message_;

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

    void handle_fuel_message(const FuelMessage& fuel_message);

    void publish_new_state(StateCode state_code);

public:
    StateWatcherFuelling(RxFuelMessagePublisher& ful_msg_pub);

    void start_working_on();

    rxcpp::observable<StateMessage> get_observable_state_message() const;

};

#endif // STATE_WATCHER_FUELLING_H
