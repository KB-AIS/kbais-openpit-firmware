#ifndef SHIFT_CONTROLLER_H
#define SHIFT_CONTROLLER_H

// qt
#include <QtCore/QDateTime>
// oss
#include <rxcpp/rx.hpp>

#include "state_watching/state_watcher_loading.h"

#include "opf/modules/sensors/gps/gps_sensor_publisher.h"

template<class duration_t>
class time_range {

    duration_t l_;

    duration_t u_;

    constexpr bool is_overlap() const noexcept { return l_ > u_; }

public:
    constexpr explicit time_range(duration_t lower, duration_t upper) noexcept
    :   l_ { lower }, u_ { upper }
    {}

    constexpr bool is_in_range(duration_t t) const noexcept {
        return !is_overlap() ? t >= l_ && t < u_ : t >= l_ || t < u_;
    }

    constexpr duration_t get_duration_to_lower(duration_t t) const noexcept {
        if (t == l_) return duration_t::zero();

        return t > l_
        ?   std::chrono::duration_cast<duration_t>(std::chrono::hours(24) + t - l_)
        :   l_ - t;
    }

    constexpr duration_t get_duration_to_upper(duration_t t) const noexcept {
        if (t == l_) return duration_t::zero();

        return t > u_
        ?   std::chrono::duration_cast<duration_t>(std::chrono::hours(24) + t - u_)
        :   u_ - t;
    }

};

struct shift_controller_config {
    std::vector<time_range<std::chrono::seconds>> shifts;

    bool use_manual_shifts { false };
};

class shift_controller_config_mapper {

};

struct shift_message {
    virtual ~shift_message() noexcept = default;
};

struct shift_message_start : public shift_message {

};

struct shift_message_close : public shift_message {

};

class i_shift_messages_publisher {

public:
    virtual ~i_shift_messages_publisher() noexcept = default;

};

class shift_controller : public i_shift_messages_publisher {

    struct shift_agregated_data {
        gps_sensor_message last_position;

        double total_mileage_m { 0.0 };
    };

    const i_gps_sensor_publisher& gps_sensor_publisher_;

    const i_state_loading_changed_publisher& state_loading_changed_publisher_;

    shift_controller_config config_;

    shift_agregated_data shift_agregated_data_;

    rxcpp::composite_subscription subs_;

    rxcpp::composite_subscription subs_shift_timers_;

    void handle_dt_sys_change();

    void handle_config_change(const shift_controller_config& config);

    void handle_setup_shift_timers();

    void handle_shift_start();

    void handle_shift_close();

    void handle_incr_total_mileage(const gps_sensor_message& msg);

public:
    explicit shift_controller(
        const i_gps_sensor_publisher& gps_sensor_publisher
    ,   const i_state_loading_changed_publisher& state_loading_changed_publisher
    );

    void start_working_on(const rxcpp::observe_on_one_worker& coordination);

};

#endif // SHIFT_CONTROLLER_H
