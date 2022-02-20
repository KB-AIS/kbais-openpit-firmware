#ifndef SERIALRXLLSSENSORPUBLISHER_H
#define SERIALRXLLSSENSORPUBLISHER_H

// qt
#include <QObject>
#include <QSerialPort>

// oss
#include "RxQt.h"

struct LlsMessage {

    char    Tem { 0 };

    quint16 Lvl { 0 };

    quint16 Frq { 0 };

};

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

    void HandleRequestSingleRead(std::vector<quint8> addresses);

};

#endif // SERIALRXLLSSENSORPUBLISHER_H
