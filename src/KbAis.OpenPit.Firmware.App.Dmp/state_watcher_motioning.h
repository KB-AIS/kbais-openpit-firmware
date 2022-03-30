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

    std::chrono::duration<int64_t, std::milli> stop_threshold {
        std::chrono::milliseconds(5'000ll)
    };

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

class StateWatcherMotioning {

    const IRxGpsSensorPublisher& gps_msg_pub_;

    StateWatcherMotioningSettings settings_;

    MotioningState current_motioning_state_;

    qint32              buffered_speed_size_ { 4 };

    qint32              buffered_speed_cur_idx_ { 0 };

    std::vector<double> buffered_speed_values_;

    rxcpp::composite_subscription subscriptions_;

    void handle_gps_message(const GpsMessage& gps_msg);

public:
    StateWatcherMotioning(const IRxGpsSensorPublisher& gps_msg_pub);

    void start_working_on();

};

#endif // STATE_WATCHER_MOTIONING_H
