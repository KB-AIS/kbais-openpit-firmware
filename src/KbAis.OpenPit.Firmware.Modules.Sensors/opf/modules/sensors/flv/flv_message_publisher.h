#ifndef FLV_MESSAGE_PUBLISHER_H
#define FLV_MESSAGE_PUBLISHER_H

// oss
#include <rxcpp/rx-observable.hpp>

struct flv_message {
    /*!
     * \brief Текущий уровень топлива в литрах, полученный при помощи
     * тарирования.
     */
    double fuel_level { 0.0 };

    /*!
     * \brief Максимальный уровень топлива в литрах.
     */
    double max_fuel_level { 0.0 };

    /*!
     * \brief Флаг указывающий, что текущий показатель уровня топлива является
     * действительным.
     */
    bool is_valid { false };
};

class i_flv_message_publisher {

public:
    virtual ~i_flv_message_publisher() noexcept = default;

    virtual rxcpp::observable<flv_message> get_observable() const = 0;
};

#endif // FLV_MESSAGE_PUBLISHER_H
