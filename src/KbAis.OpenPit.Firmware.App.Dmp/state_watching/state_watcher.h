#ifndef STATE_WATCHER_H
#define STATE_WATCHER_H

// std
#include <chrono>
// oss
#include <rxcpp/rx.hpp>

namespace rx {

using namespace rxcpp;
using namespace rxcpp::operators;
using namespace rxcpp::sources;
using namespace rxcpp::util;

}

/*!
 *  \brief Состояния в которые может переходить оборудование.
 */
enum class state_code : unsigned char {
    /*!
     *  \brief Неизвестное состояние
     * 
     *  Используется, когда невозможно определить состояние оборудования, 
     *  например, при инициализации прошивки.
     */
    unknown

    /*!
     * \brief Состояние начла движения.
     */
,   moving_begin
    /*!
     *  \brief Состояние начала остановки.
     *
     *  Переход в состояние начала остановки возможен, если предыдущее 
     *  состояние было начало движения.
     */
,   staying_begin
    /*!
     *  \brief Состояние начала парковки.
     */
,   parking_begin

    /*!
     *  \brief Состояние начала загрузки.
     */
,   loading_begin
    /*!
     *  \brief Состояния окончания загрузки.
     */
,   loading_finish
    /*!
     *  \brief Состояние начала отвала.
     */
,   unloading_begin
    /*!
     *  \brief Состояние окончания отвала.
     */
,   unloading_finish

    /*!
     * \brief Состояние начала заправки.
     */
,   refueling_begin
    /*!
     * \brief Состояние окончания заправки.
     */
,   refueling_finish
    /*!
     * \brief Состояние начала слива топлива.
     */
,   discharging_begin
    /*!
     * \brief Состояние окончания слива топлива.
     */
,   discharging_finish
};

/*!
 *  \brief Сообщение о изменении текущего состояния оборудования.
 */
struct state_changed_message {
    state_code new_state_code;
};

/*!
 *  \brief
 */
class i_state_watcher_service {

public:
    virtual ~i_state_watcher_service() noexcept = default;

    /*!
     *  \brief Запуск работы сервиса.
     *
     *  Запуск работы сервиса. по отслеживанию текущего состояния оборудования 
     *  согласно показателям внешних систем.
     */
    virtual void start_working_on(const rxcpp::observe_on_one_worker& coordination) = 0;
};

///*!
// *  \brief
// */
//class i_state_changed_publisher {

//public:
//    virtual ~i_state_changed_publisher() noexcept = default;

//    /*!
//     *  \brief Получение источника сообщений.
//     *
//     *  Получение источника публикации сообщений об изменении текущего
//     *  состояния оборудования.
//     */
//    virtual rxcpp::observable<state_changed_message> get_observable() const = 0;
//};


#endif // STATE_WATCHER_H
