#ifndef STATE_WATCHER_MOTIONING_H
#define STATE_WATCHER_MOTIONING_H

// std
#include <chrono>

#include "IRxGpsSensorPublisher.h"
#include "RxQt.h"

class IStateWatcher {

public:
    virtual ~IStateWatcher();

    /*!
     *  \brief Запуск работы сервиса по отслеживанию текущего состояния
     *  оборудования согласно показателям внешних систем.
     */
    void start_working_on();

};

struct StateWatcherMotioningSettings {

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
    double min_speed_to_move { 0.75 };

};

enum class MotioningState { Move, Stop, Park };

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
class StateWatcherMotioning {
    using SteadyClock_t = std::chrono::steady_clock::time_point;

    using MotioningState_t = rxcpp::rxsub::behavior<MotioningState>;

    const IRxGpsSensorPublisher& gps_msg_pub_;

    StateWatcherMotioningSettings settings_;

    rxcpp::composite_subscription subscriptions_;

    MotioningState_t current_motioning_state_;

    int                 buffered_speed_size_ { 4 };

    int                 buffered_speed_cur_idx_ { 0 };

    std::vector<double> buffered_speed_values_;

    SteadyClock_t last_move_time;

    SteadyClock_t last_stop_time;

    SteadyClock_t prev_stop_time;

    SteadyClock_t last_park_time;

    void handle_gps_message(const GpsMessage& gps_msg);

    bool is_in_move_state() const;

    bool is_in_stop_state() const;

    bool is_in_park_state() const;

public:
    StateWatcherMotioning(const IRxGpsSensorPublisher& gps_msg_pub);

    void start_working_on();

};

#endif // STATE_WATCHER_MOTIONING_H
