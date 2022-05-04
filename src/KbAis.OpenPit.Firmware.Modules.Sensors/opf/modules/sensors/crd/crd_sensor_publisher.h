#ifndef CRD_SENSOR_PUBLISHER_H
#define CRD_SENSOR_PUBLISHER_H

// std
#include <chrono>
// qt
#include <QSerialPort>

#include "rxqt.h"

#include "opf/modules/sensors/crd/crd_formatter_tkcardreader_modbus.h"

enum class crd_sensor_state {
    NoResponse
,   ResponseInvalid
,   Response = -1
};

struct crd_sensor_message {

    /*!
     * \brief Сетевой адрес Modbus устройства.
     */
    quint8 network_address;

    /*!
     * \brief Полученный в ответ на запрос чтения из регистров номер карты.
     */
    quint32 card_number;

};

class i_crd_sensor_publisher {

public:
    virtual rxcpp::observable<crd_sensor_message> get_observable(const rxcpp::observe_on_one_worker& c) const = 0;

};

struct crd_publisher_settings {

    QString interface;

    unsigned char network_address;

    std::chrono::duration<double> repeated_read_filter_interval;

};

struct crd_request_meta {

    std::optional<crd_response_cardnumber> response;

    bool got_any_data { false };

    bool has_decode_rsp_err { false };

};

struct crd_publish_meta {

    crd_response_cardnumber first_uniqe_response ;

    std::chrono::steady_clock::time_point first_uniqe_read_time;

};

/*!
 * \brief Сервис получения данных с устройства TK-CardReader Plus через серийный интерфейс и их
 * публикации в виде сообщений.
 */
class crd_sensor_publisher : public QObject, public i_crd_sensor_publisher {

    crd_request_meta request_meta_;

    crd_publish_meta publish_meta_;

    crd_publisher_settings m_settings;

    QSerialPort serialport_crd_;

    QTimer timer_request_card_number_;

    rxcpp::rxsub::behavior<crd_sensor_message> publisher_;

    void config_serialport_connection();

    void send_request_card_number();

    void publish_card_number();

    void handle_ready_read();

public:
    crd_sensor_publisher();

    void start_work_on(const rxcpp::observe_on_one_worker& coord);

    rxcpp::observable<crd_sensor_message> get_observable(const rxcpp::observe_on_one_worker& coord) const override;
};

#endif // CRD_SENSOR_PUBLISHER_H
