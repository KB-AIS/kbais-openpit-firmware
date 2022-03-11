#ifndef SERIALRXRCRSENSORPUBLISHER_H
#define SERIALRXRCRSENSORPUBLISHER_H

// qt
#include <QSerialPort>

#include "RxQt.h"

/*!
 * Service for reading data from TK-CardReader Plus via serial interface and
 * publishing parsed messages.
 */
class SerialRxRcrSensorPublisher : public QObject {

    QSerialPort* m_device;

    QTimer* m_timer;

    void configure_device_connection();

    void request_data();

    void handle_ready_read();

public:
    SerialRxRcrSensorPublisher();

    void start_work_on();

};

#endif // SERIALRXRCRSENSORPUBLISHER_H
