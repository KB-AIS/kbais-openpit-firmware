#ifndef SHIFT_CONTROLLER_H
#define SHIFT_CONTROLLER_H

// qt
#include <QtCore/QDateTime>
// oss
#include <rxcpp/rx.hpp>

struct shift_start_message {

};

struct shift_close_message {

};

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

    constexpr duration_t get_duration_to_lower(duration_t t) const {
        if (t == l_) return duration_t::zero();

        return t > l_
        ?   std::chrono::duration_cast<duration_t>(std::chrono::hours(24) + t - l_)
        :   l_ - t;
    }

    constexpr duration_t get_duration_to_upper(duration_t t) const {
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

class shift_controller {

    struct shift_agregated_data {

    };

    shift_controller_config config_;

    rxcpp::composite_subscription subs_;

    rxcpp::composite_subscription subs_t_;

    void handle_dt_sys_change();

    void handle_config_change();

    void handle_setup_shift_close_timer();

    void handle_shift_start();

    void handle_shift_close();

public:
    explicit shift_controller();

    void start_working_on(const rxcpp::observe_on_one_worker& coordination);

};

#endif // SHIFT_CONTROLLER_H
