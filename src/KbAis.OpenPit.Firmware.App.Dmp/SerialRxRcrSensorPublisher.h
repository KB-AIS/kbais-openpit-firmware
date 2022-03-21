#ifndef SERIALRXRCRSENSORPUBLISHER_H
#define SERIALRXRCRSENSORPUBLISHER_H

// std
#include <chrono>
// qt
#include <QSerialPort>

#include "FormatterModbusCardReader.h"
#include "RxQt.h"

enum class CardReaderState {
    NoResponse
,   ResponseInvalid
,   Response = -1
};

struct CardReaderMessage {

    /*!
     * \brief Сетевой адрес Modbus устройства.
     */
    quint8 card_reader_net_adr;

    /*!
     * \brief Полученный в ответ на запрос чтения из регистров номер карты.
     */
    quint32 card_number;

};

class IRxPublisherCardReader {

public:
    virtual rxcpp::observable<CardReaderMessage> get_observable() const = 0;

};

struct PublisherCardReaderSettings {

    QString interface;

    quint8  network_address;

    std::chrono::duration<double> repeated_read_filter_interval;

};

struct CardReaderRequestMeta {

    std::optional<ResponseCardNumber> response;

    bool got_any_data { false };

    bool has_decode_rsp_err { false };

};

struct CardReaderPubslihMeta {

    ResponseCardNumber first_uniqe_response ;

    std::chrono::steady_clock::time_point first_uniqe_read_time;

};

/*!
 * Service for reading data from TK-CardReader Plus via serial interface and
 * publishing parsed messages.
 */
class RxServiceCardReader : public QObject, public IRxPublisherCardReader {

    CardReaderRequestMeta m_request_meta;

    CardReaderPubslihMeta m_publish_meta;

    PublisherCardReaderSettings m_settings;

    QSerialPort m_serialport_cardreader;

    QTimer m_timer_request_card_number;

    rxcpp::rxsub::behavior<CardReaderMessage> m_publisher;

    void config_device_connection();

    void start_work_internal();

    void send_request_card_number();

    void publish_card_number();

    void handle_ready_read();

public:
    RxServiceCardReader();

    void start_work_on();

    rxcpp::observable<CardReaderMessage> get_observable() const override;
};

#endif // SERIALRXRCRSENSORPUBLISHER_H
