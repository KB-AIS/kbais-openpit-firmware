#include "RxStateWatcherFueling.h"

// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

#include "Format.h"

using namespace std::chrono;

constexpr int FUL_LVL_BUF_SIZE { 5 };
// TODO: Get from config
constexpr int FUL_LVL_PERIOD { 50 };

constexpr double FUL_LVL_WIDTH_FIX { 5.0 };

constexpr double FUL_LVL_FUELLING { 20.0 };

constexpr double FUL_LVL_FUELLING_DLT { 10.0 };

constexpr double FUL_LVL_DRAINING { 20.0 };

constexpr double FUL_LVL_DRAINGIN_DLT { 10.0 };


RxStateWatcherFueling::RxStateWatcherFueling(RxFuelMessagePublisher& ful_msg_pub)
    :   m_ful_msg_pub(ful_msg_pub)
    ,   m_subject_state_message(StateMessage { StateCode::Unknown })
{
    m_fls.resize(FUL_LVL_BUF_SIZE);
}

void
RxStateWatcherFueling::start_working_on() {
    m_ful_msg_pub.get_obeservable_fuel_message()
        .subscribe([this](const auto& x) { handle_fuel_message(x); });
}

void
RxStateWatcherFueling::handle_fuel_message(const FuelMessage& fuel_message) {
    m_fls[m_fl_current_idx] = fuel_message.cur_fuel_level;
    m_fl_current_idx = (m_fl_current_idx + 1) % FUL_LVL_BUF_SIZE;

    const auto fl_average = ranges::accumulate(m_fls, 0.0) / FUL_LVL_BUF_SIZE;

    // Q!: Some magic is happening here, need to add an detailed comment
    m_fl_filtered = m_fl_filtered > 0.0
        ? m_fl_filtered * (FUL_LVL_PERIOD - 1) / FUL_LVL_PERIOD + fl_average / FUL_LVL_PERIOD
        : fl_average;

    const auto fl_delta = std::abs(fl_average - m_fl_filtered);

    // TODO: Move to 'fixing' function
    if (fl_delta < FUL_LVL_WIDTH_FIX) {
        const auto need_fix =
            !m_fl_fixed
            && duration_cast<milliseconds>(steady_clock::now() - m_fl_last_unfixed_time) > 5s;

        if (need_fix) {
            m_fl_fixed = true;

            if (!m_state_fueling) {
                m_fl_fixed_tmp = fl_average;
            }
        }
    } else {
        m_fl_last_unfixed_time = steady_clock::now();
        const auto need_unfix = fl_delta > 2.0 * FUL_LVL_WIDTH_FIX;

        if (need_unfix) {
            m_fl_fixed = false;
        }
    }

    // TODO: Move to 'fueling' function
    const auto fl_fueling = fl_average - m_fl_filtered;
    if (fl_fueling >= FUL_LVL_FUELLING && !m_state_fueling) {
        m_state_fueling = true;
        m_fl_fueling_begin = m_fl_fixed_tmp;
        publish_new_state(StateCode::RefuelBegin);
    } else if (fl_fueling < FUL_LVL_FUELLING - FUL_LVL_FUELLING_DLT && m_state_fueling) {
        m_state_fueling = false;
        publish_new_state(StateCode::RefuelFinish);
    }

    // TODO: Move to 'draining' function
    const auto fl_draingin = m_fl_filtered - fl_average;
    if (fl_draingin >= FUL_LVL_DRAINING && !m_state_draining) {
        m_state_draining = true;
        m_fl_draining_begin = m_fl_filtered;
        publish_new_state(StateCode::DrainBegin);
    } else if (fl_draingin < FUL_LVL_DRAINING - FUL_LVL_DRAINGIN_DLT && m_state_draining) {
        m_state_draining = false;
        publish_new_state(StateCode::DrainFinish);
    }
}

void
RxStateWatcherFueling::publish_new_state(StateCode state_code) {
    m_subject_state_message.get_subscriber().on_next(StateMessage { state_code });
}

rxcpp::observable<StateMessage>
RxStateWatcherFueling::get_observable_state_message() const {
    return m_subject_state_message.get_observable();
}
