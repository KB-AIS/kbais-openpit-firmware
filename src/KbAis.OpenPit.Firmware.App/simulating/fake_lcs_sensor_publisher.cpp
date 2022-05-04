#include "fake_lcs_sensor_publisher.h"

// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

using namespace ranges;
using namespace rx;
using namespace std::chrono;
using namespace std::chrono_literals;

constexpr auto LCS_MES_SEND_INTERVAL { 500ms };

std::mt19937 lcs_random_engine;

fake_lcs_sensor_publisher::fake_lcs_sensor_publisher() noexcept
{
}

rxcpp::observable<lcs_sensor_message> fake_lcs_sensor_publisher::get_observable(
    const rxcpp::observe_on_one_worker& c
) const {
    return sub_lcs_sensor_messages_.get_observable().subscribe_on(c);
}

void fake_lcs_sensor_publisher::setup_scenario(
    [[maybe_unused]] const rxcpp::observe_on_one_worker& c
) {
    constexpr auto gen_lcs_msgs_smo = [](
        double wgt_s, double wgt_e
    ,   duration<int64_t, std::milli> duration
    ) -> std::vector<lcs_sensor_message> {
        constexpr auto mke_lcs_msg = [](double wgt) -> lcs_sensor_message {
            return { .weight = wgt, .weight_raw = 0.0, .is_valid = true };
        };

        const auto msgs_num = duration / LCS_MES_SEND_INTERVAL;

        return
            views::linear_distribute(wgt_s, wgt_e, msgs_num)
        |   views::transform([&](double x) { return mke_lcs_msg(x); })
        |   to<std::vector>();
    };

    constexpr auto gen_lcs_msgs_dlt = [](
        double wgt
    ,   double wgt_dlt = 0.0
    ,   duration<int64_t, std::milli> dur = 1000ms
    ) -> std::vector<lcs_sensor_message> {
        constexpr auto mke_lcs_msg = [](double wgt) -> lcs_sensor_message {
            return { .weight = wgt, .weight_raw = 0.0, .is_valid = true };
        };

        const auto wgt_bnd_dif = wgt * wgt_dlt / 100.0;
        const auto wgt_bnd_lwr = wgt - wgt_bnd_dif, wgt_bnd_upr = wgt + wgt_bnd_dif;
        const auto lcs_msgs_num = dur / LCS_MES_SEND_INTERVAL;
        const auto lcs_msgs_hlf = lcs_msgs_num / 2, lcs_msgs_dbl = lcs_msgs_num * 2;

        const auto wgts_lwr =
            views::linear_distribute(wgt_bnd_lwr, wgt, lcs_msgs_dbl)
        |   views::sample(lcs_msgs_hlf, lcs_random_engine);

        const auto wgts_upd =
            views::linear_distribute(wgt, wgt_bnd_upr, lcs_msgs_dbl)
        |   views::sample(lcs_msgs_hlf, lcs_random_engine);

        return views::concat(wgts_lwr, wgts_upd)
        |   views::transform(mke_lcs_msg)
        |   to<std::vector<lcs_sensor_message>>()
        |   actions::shuffle(lcs_random_engine);
    };

    auto delay_every =
        concat_map([](auto x) { return just(x) | delay(LCS_MES_SEND_INTERVAL); });

    obs_lcs_sensor_messages_ =
        from(
            c
        ,   iterate(gen_lcs_msgs_smo(1.0 * 100.0, 200.0 * 100.0, 15s))
        ,   iterate(gen_lcs_msgs_dlt(200.0 * 100.0, 1.33, 50s))
        ,   iterate(gen_lcs_msgs_smo(1.0 * 100.0, 100.0 * 100.0, 11s) | actions::reverse)
        ,   iterate(gen_lcs_msgs_dlt(10.0 * 100.0, 1.0, 40s))
        )
    |   concat() | delay_every | repeat() | multicast(sub_lcs_sensor_messages_);
}

void fake_lcs_sensor_publisher::start_scenario() {
    obs_lcs_sensor_messages_.connect();
}
