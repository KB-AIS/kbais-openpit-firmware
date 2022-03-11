#ifndef SERIALRXLCSSENSORPUBLISHER_H
#define SERIALRXLCSSENSORPUBLISHER_H

// qt
#include <QObject>
#include <QSerialPort>

#include <RxQt.h>

class SerialRxLcsSensorPublisher : public QObject {
    Q_OBJECT

    QSerialPort m_device;

    void handle_device_configure();

    void handle_device_ready_read();

    void handle_send_request();

public:
    SerialRxLcsSensorPublisher();
    
    ~SerialRxLcsSensorPublisher() noexcept;

    void start_publish_on(const rxcpp::observe_on_one_worker& worker);

};

#endif // SERIALRXLCSSENSORPUBLISHER_H
