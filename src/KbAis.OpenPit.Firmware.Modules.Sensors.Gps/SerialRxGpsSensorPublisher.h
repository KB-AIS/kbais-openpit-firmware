#ifndef SERIALRXGPSSENSORPUBLISHER_H
#define SERIALRXGPSSENSORPUBLISHER_H

// std
#include <memory>
#include <vector>
// qt
#include <QObject>
#include <QtSerialPort/QSerialPort>
//oss
#include <rxcpp/rx.hpp>

#include "GpsMessage.h"
#include "IRxGpsSensorPublisher.h"
#include "Nmea/NmeaParser.h"

class SerialRxGpsSensorPublisher : public IRxGpsSensorPublisher {

public:
    SerialRxGpsSensorPublisher();

    ~SerialRxGpsSensorPublisher();

    const rxcpp::observable<GpsMessage> GetObservable() const override;

private:
    QSerialPort serialGpsSensor;

    rxcpp::subjects::behavior<GpsMessage> subject;

    std::vector<std::shared_ptr<NmeaSentence>> nmeaSentences;

    std::shared_ptr<GgaSentence> lastGgaSentence;

    std::shared_ptr<RmcSentence> lastRmcSentence;

    void SetupGpsDevice();

    bool ResetGpsDevice();

    void OnGpsSensorReadyRead();

    bool NeedPublishGpsMessage() const;

    GpsMessage MapGpsMessage() const;

};

#endif // SERIALRXGPSSENSORPUBLISHER_H
