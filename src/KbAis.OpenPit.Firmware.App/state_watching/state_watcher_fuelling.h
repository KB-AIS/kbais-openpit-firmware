#ifndef STATE_WATCHER_FUELLING_H
#define STATE_WATCHER_FUELLING_H

// std
#include <chrono>

#include "state_watching/state_watcher_motioning.h"

#include "opf/modules/sensors/flv/flv_calibration_publisher.h"

struct state_watcher_fuelling_configuration {

};

/*!
 *  \brief Сервис мониторинга за уровнем топлива оборудования и детекции
 *  заправок/сливов.
 *
 *  \note Оригинальная реализаця взята из tiptruck\fuel.cpp.
 */
class state_watcher_fuelling : public i_state_watcher_service {
    using fuelling_state_t = rxcpp::rxsub::behavior<state_changed_message>;

    using time_point_t = std::chrono::steady_clock::time_point;

    struct impl_tk {
        std::vector<double> buffered_flv;
        int                 buffered_flv_cur_idx { 0 };

        double              flv_filtered         { 0.0 };

        bool                is_flv_fixed         { false };
        time_point_t        tp_flv_last_unfix    { std::chrono::steady_clock::now() };
        double              flv_fixed_tmp        { 0.0 };

        bool                is_state_fueling     { false };
        bool                is_state_draining    { false };
    };

    const flv_calibration_publisher& flv_calibration_publisher_;

    fuelling_state_t current_fuelling_state_;

    impl_tk tk_;

    void handle_flv_message(const flv_message& msg);

public:
    state_watcher_fuelling(const flv_calibration_publisher& flv_calibration_publisher);

    void start_working_on(
        const rxcpp::observe_on_one_worker& coordination
    ) override;

    rxcpp::observable<state_changed_message> get_observable() const;

};

#endif // STATE_WATCHER_FUELLING_H
