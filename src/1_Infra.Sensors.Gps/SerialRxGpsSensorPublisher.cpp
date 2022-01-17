#include "SerialRxGpsSensorPublisher.h"

// std
#include <typeinfo>
#include <functional>
// qt
#include <QtGlobal>

#include "Casting.h"

// !Q: where device name comes from?
static const QString DEVICE_PORT_NAME { "/dev/ttyS5" };

SerialRxGpsSensorPublisher::SerialRxGpsSensorPublisher()
    : serialGpsSensor { this }
    , subject { GpsMessage {} }
    , lastGgaSentence { std::make_shared<GgaSentence>() }
    , lastRmcSentence { std::make_shared<RmcSentence>() }
{
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
        Utils::Casting::action_if<GgaSentence>(sentence, [=](auto sentence) mutable {
            lastGgaSentence = sentence;
        });

        Utils::Casting::action_if<RmcSentence>(sentence, [=](auto sentence) mutable {
            lastRmcSentence = sentence;
        });

        if (!needPublishGpsMessage()) continue;

        subject.get_subscriber().on_next(mapGpsMessage());
    }
}

bool
SerialRxGpsSensorPublisher::needPublishGpsMessage() const {
    // Check if sentences have valid time
    if (!lastGgaSentence->time.isValid() || !lastRmcSentence->datetime.isValid()) {
        return false;
    }

    // Check if sentences from same receive frame
    if (lastGgaSentence->time != lastRmcSentence->datetime.time()) {
        return false;
    }

    return true;
}

GpsMessage
SerialRxGpsSensorPublisher::mapGpsMessage() const {
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
