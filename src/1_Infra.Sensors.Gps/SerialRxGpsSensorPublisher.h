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

#include "Nmea/NmeaParser.h"
#include "GpsMessage.h"
#include "IRxGpsSensorPublisher.h"

class SerialRxGpsSensorPublisher : public IRxGpsSensorPublisher {

public:
    SerialRxGpsSensorPublisher();

    ~SerialRxGpsSensorPublisher();

    const rxcpp::observable<GpsMessage> getObservable() const override;

private:
    QSerialPort serialGpsSensor;

    rxcpp::subjects::behavior<GpsMessage> subject;

    std::vector<std::shared_ptr<NmeaSentence>> nmeaSentences;

    std::shared_ptr<GgaSentence> lastGgaSentence;

    std::shared_ptr<RmcSentence> lastRmcSentence;

    void setupGpsDevice();

    bool resetGpsDevice();

    void handleGpsSensorRead();

    GpsMessage mapGpsUpdate() const;

};

#endif // SERIALRXGPSSENSORPUBLISHER_H
