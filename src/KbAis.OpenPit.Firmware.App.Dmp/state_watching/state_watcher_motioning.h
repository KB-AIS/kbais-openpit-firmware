#ifndef STATE_WATCHER_MOTIONING_H
#define STATE_WATCHER_MOTIONING_H

#include "IRxGpsSensorPublisher.h"
#include "presentation/dmp/state_changed_publisher.h"
#include "state_watching/state_watcher.h"

struct state_watcher_motioning_configuration {

    /*!
     * \brief Продолжительность в течение которого оборудование должно
     * оставаться неподвижным, чтобы зафиксировать состоянии «остановки».
     */
    std::chrono::duration<int64_t, std::milli> stop_threshold {
        std::chrono::milliseconds(5'000ll)
    };

    /*!
     * \brief Продолжительность в течение которого оборудование должно
     * оставаться неподвижным, чтобы зафиксировать состоянии «парковки».
     */
    std::chrono::duration<int64_t, std::milli> park_threshold {
        std::chrono::milliseconds(30'000ll)
    };

    /*!
     * \brief Минимальное значение скорости для детектирования движения
     * оборудования. Значение в км/ч.
     */
    double min_speed_to_move { 5.0 };

};

/*!
 * \brief Сервис мониторинга изменения состояния движения оборудования.
 *
 * Основным параметром мониторинга является показатель скорости, получаемый с
 * датчика GPS. Сервис детектирует изменение значения скорости за временной
 * промежуток, и в соответствии с заданными параметрами в экземпляре
 * \ref StateWatcherMotioningSettings публикует событие изменения состояния
 * движения.
 *
 */
class state_watcher_motioning : public i_state_watcher_service, public i_state_changed_publisher {
    using steady_clock_t = std::chrono::steady_clock::time_point;

    using motioning_state_t = rxcpp::rxsub::behavior<state_code_motioning>;

    const i_gps_sensor_publisher& gps_sensor_publisher_;

    state_watcher_motioning_configuration configuration_;

    rxcpp::composite_subscription subscriptions_;

    motioning_state_t current_motioning_state_;

    int                 buffered_speed_size_ { 4 };

    int                 buffered_speed_cur_idx_ { 0 };

    std::vector<double> buffered_speed_values_;

    steady_clock_t last_move_time;

    steady_clock_t last_stop_time;

    steady_clock_t prev_stop_time;

    steady_clock_t last_park_time;

    void handle_gps_message(const GpsMessage& gps_msg);

    bool is_in_move_state() const;

    bool is_in_stop_state() const;

    bool is_in_park_state() const;

public:
    state_watcher_motioning(const i_gps_sensor_publisher& gps_sensor_publisher);

    void start_working_on(
        const rxcpp::observe_on_one_worker& coordination
    ) override;

    rxcpp::observable<state_code_motioning> get_observable(
        const rxcpp::observe_on_one_worker& coordination
    ) const override;

};

#endif // STATE_WATCHER_MOTIONING_H
