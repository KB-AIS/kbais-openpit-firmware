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
#include "GpsUpdateDto.h"
#include "IRxGpsSensorPublisher.h"

class SerialRxGpsSensorPublisher : public IRxGpsSensorPublisher {

public:
    SerialRxGpsSensorPublisher();

    ~SerialRxGpsSensorPublisher();

    const rxcpp::observable<GpsUpdateDto> getObservable() const override;

private:
    QSerialPort serialGpsSensor;

    rxcpp::subjects::behavior<GpsUpdateDto> subject;

    std::vector<std::shared_ptr<NmeaSentence>> nmeaSentences;

    std::shared_ptr<GgaSentence> lastGgaSentence;

    std::shared_ptr<RmcSentence> lastRmcSentence;

    void setupGpsDevice();

    bool resetGpsDevice();

    void handleGpsSensorRead();

    GpsUpdateDto mapGpsUpdate() const;

};

#endif // SERIALRXGPSSENSORPUBLISHER_H
