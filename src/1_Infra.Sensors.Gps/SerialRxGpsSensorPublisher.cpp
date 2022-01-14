#include "SerialRxGpsSensorPublisher.h"

// std
#include <typeinfo>
#include <functional>
// qt
#include <QtGlobal>

#include "Casting.h"

// !Q: where device name comes from?
static const QString DEVICE_PORT_NAME { "/dev/ttyS5" };

SerialRxGpsSensorPublisher::SerialRxGpsSensorPublisher() :
    serialGpsSensor { this }, subject { GpsMessage {} } {
    connect(
        &serialGpsSensor, &QSerialPort::readyRead,
        this, &SerialRxGpsSensorPublisher::handleGpsSensorRead
    );

    setupGpsDevice();
    resetGpsDevice();
}

SerialRxGpsSensorPublisher::~SerialRxGpsSensorPublisher() {
    if (!serialGpsSensor.isOpen()) return;

    serialGpsSensor.close();
}

const rxcpp::observable<GpsMessage>
SerialRxGpsSensorPublisher::getObservable() const {
    return subject.get_observable();
}

void
SerialRxGpsSensorPublisher::setupGpsDevice() {
    serialGpsSensor.setDataBits(QSerialPort::Data8);
    serialGpsSensor.setFlowControl(QSerialPort::NoFlowControl);
    serialGpsSensor.setParity(QSerialPort::NoParity);
    serialGpsSensor.setPortName(DEVICE_PORT_NAME);
    serialGpsSensor.setStopBits(QSerialPort::OneStop);
}

bool SerialRxGpsSensorPublisher::resetGpsDevice() {
    serialGpsSensor.setBaudRate(QSerialPort::Baud9600);

    // TOOD: Reset power on pins
    auto opened = serialGpsSensor.open(QSerialPort::ReadOnly);

    return opened;
}

void
SerialRxGpsSensorPublisher::handleGpsSensorRead() {
    // Remove sentences from read buffer
    nmeaSentences.clear();
    processDeviceRead(serialGpsSensor, nmeaSentences);

    for (auto sentence : nmeaSentences) {
        // TODO: Looks like we can replace action_if with Boost::Hana
        Utils::Casting::action_if<GgaSentence>(sentence, [=](auto sentence) {
            lastGgaSentence = sentence;
        });

        Utils::Casting::action_if<RmcSentence>(sentence, [=](auto sentence) {
            lastRmcSentence = sentence;
        });

        if (!lastGgaSentence || !lastRmcSentence) continue;

        // Skip update if GGA and RMC were not collected at the same time.
        if (lastGgaSentence->time != lastRmcSentence->datetime.time()) continue;

        subject.get_subscriber().on_next(mapGpsUpdate());
    }
}

GpsMessage
SerialRxGpsSensorPublisher::mapGpsUpdate() const {
    return GpsMessage {
        lastRmcSentence->datetime,
        lastRmcSentence->isValid,
        lastRmcSentence->latitude,
        lastRmcSentence->longitude,
        lastRmcSentence->speed,
        lastGgaSentence->gpsQauality,
        lastGgaSentence->satellites
    };
}
