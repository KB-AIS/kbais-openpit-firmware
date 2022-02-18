#ifndef SERIALRXLLSSENSORPUBLISHER_H
#define SERIALRXLLSSENSORPUBLISHER_H

// qt
#include <QObject>
#include <QSerialPort>

// oss
#include "RxQt.h"

class SerialRxLlsSensorPublisher : public QObject {
    Q_OBJECT

public:
    SerialRxLlsSensorPublisher();

    void StartWorking();

private:
    QSerialPort m_spLlsDevice;

    QTimer      m_tmLlsReadRequest;

    void ConfigLlsDeviceConnection();

    void HandleReadyRead();

    void HandleReadRequest();

};

#endif // SERIALRXLLSSENSORPUBLISHER_H
