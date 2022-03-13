#ifndef SERIALRXRCRSENSORPUBLISHER_H
#define SERIALRXRCRSENSORPUBLISHER_H

// qt
#include <QSerialPort>

#include "RxQt.h"

class IDeviceWorker {

public:
    virtual void start_work_on() = 0;

};

struct CardReadMessage {

    quint32 cdn;

};

class IRxMessagePublisherCardReader {

public:
    virtual rxcpp::observable<CardReadMessage> get_observable() const = 0;

};

/*!
 * Service for reading data from TK-CardReader Plus via serial interface and
 * publishing parsed messages.
 */
class SerialRxRcrSensorPublisher : public QObject {

    QSerialPort m_sp_device;

    QTimer m_tm_send_req_read_card_number;

    void config_device_connection();

    void start_work_internal();

    void send_req_read_card_number();

    void handle_ready_read();

public:
    SerialRxRcrSensorPublisher();

    void start_work_on();

};

#endif // SERIALRXRCRSENSORPUBLISHER_H
