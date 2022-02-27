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

RxStateWatcherFueling::RxStateWatcherFueling(RxFuelMessagePublisher& ful_msg_pub)
    :   m_ful_msg_pub(ful_msg_pub)
{
    m_ful_lvl_buf.resize(FUL_LVL_BUF_SIZE);
}

void
RxStateWatcherFueling::start_working_on() {
    m_ful_msg_pub
        .get_obeservable_fuel_message()
        .subscribe([this](const auto& x) { handle_fuel_message(x); });
}

void
RxStateWatcherFueling::handle_fuel_message(const FuelMessage& fuel_message) {
    m_ful_lvl_buf[m_ful_lvl_buf_idx] = fuel_message.cur_fuel_level;
    m_ful_lvl_buf_idx = (m_ful_lvl_buf_idx + 1) % FUL_LVL_BUF_SIZE;

    const auto ful_lvl_avg = ranges::accumulate(m_ful_lvl_buf, 0.0) / FUL_LVL_BUF_SIZE;

    // Q!: Some magic is happening here, need to add an detailed comment
    m_ful_lvl_filter = m_ful_lvl_filter > 0.0
        ? m_ful_lvl_filter * (FUL_LVL_PERIOD - 1) / FUL_LVL_PERIOD + ful_lvl_avg / FUL_LVL_PERIOD
        : ful_lvl_avg;

    const auto ful_lvl_dlt = std::abs(ful_lvl_avg - m_ful_lvl_filter);

    // WTF
    // TODO: Move to 'fixing' function
    if (ful_lvl_dlt < FUL_LVL_WIDTH_FIX) {
        const auto need_set_ful_lvl_fix =
            !m_ful_lvl_fix
            && duration_cast<milliseconds>(steady_clock::now() - m_ful_lvl_fix_reset) > 5s;

        if (need_set_ful_lvl_fix) {
            m_ful_lvl_fix = true;

            if (!m_state_fueling) m_ful_lvl_tmp = ful_lvl_avg;
        }
    } else {
        m_ful_lvl_fix_reset = steady_clock::now();
        const auto need_res_ful_lvl_fix = ful_lvl_dlt > 2.0 * FUL_LVL_WIDTH_FIX;

        if (need_res_ful_lvl_fix) m_ful_lvl_fix = false;
    }

    // TODO: Move to 'fueling' function
    if ((ful_lvl_avg - m_ful_lvl_filter) >= 20 && !m_state_fueling) {
        m_state_fueling = true;
        m_fueling_begin = m_ful_lvl_tmp;
        PLOGD << "State fueling begin -- " << m_fueling_begin;
    } else if ((ful_lvl_avg - m_ful_lvl_filter) < (20 - 10) && m_state_fueling) {
        m_state_fueling = false;
        PLOGD << "State fueling end -- " << ful_lvl_avg - m_fueling_begin;
    }

    if (m_ful_lvl_filter - ful_lvl_avg >= 20 && !m_state_draining) {
        m_state_draining = true;
        m_draining_begin = m_ful_lvl_filter;
        PLOGD << "State draining begin -- " << m_draining_begin;
    } else if (m_ful_lvl_filter - ful_lvl_avg < 20 - 10 && m_state_draining) {
        m_state_draining = false;
        PLOGD << "State drainig end -- " << m_draining_begin - ful_lvl_avg;
    }

    PLOGD << fmt::format("State watcher fueling: avg {:f} dlt {:f} flt {:f} fix {}", ful_lvl_avg, ful_lvl_dlt, m_ful_lvl_filter, m_ful_lvl_fix);
}
