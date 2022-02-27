#ifndef RXSTATEWATCHERFUELING_H
#define RXSTATEWATCHERFUELING_H

// std
#include <chrono>

#include "Modules/Sensors/Serials/RxFuelMessagePublisher.h"

class RxStateWatcherFueling {
    using SteadyClock_t = std::chrono::steady_clock::time_point;


    RxFuelMessagePublisher& m_ful_msg_pub;

    std::vector<double> m_ful_lvl_buf;
    int                 m_ful_lvl_buf_idx { 0 };
    double              m_ful_lvl_filter  { 0.0 };

    bool                m_ful_lvl_fix { false };
    SteadyClock_t       m_ful_lvl_fix_reset;
    double              m_ful_lvl_tmp     { 0.0 };

    bool                m_state_fueling { false };
    double              m_fueling_begin { 0.0 };

    bool                m_state_draining { false };
    double              m_draining_begin { 0.0 };

    void handle_fuel_message(const FuelMessage& fuel_message);

public:
    RxStateWatcherFueling(RxFuelMessagePublisher& ful_msg_pub);

    void start_working_on();

};

#endif // RXSTATEWATCHERFUELING_H
