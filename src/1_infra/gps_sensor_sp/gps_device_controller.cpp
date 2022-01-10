#include "gps_device_controller.h"

// std
#include <typeinfo>
#include <functional>
// qt
#include <QtGlobal>

#include "casting.h"

using namespace KbAis::Cfw::Sensors::Gps;

// !Q: where device name comes from?
static const QString DEVICE_PORT_NAME { "/dev/ttyS5" };

SerialPortGpsDeviceController::SerialPortGpsDeviceController() :
    spGpsDevice(new QSerialPort), sentences() {
    connect(
        spGpsDevice, &QSerialPort::readyRead,

        this, &SerialPortGpsDeviceController::handleGpsDeviceReadyRead
    );

    setupGpsDevice();
    resetGpsDevice();
}

SerialPortGpsDeviceController::~SerialPortGpsDeviceController() {
    if (!spGpsDevice->isOpen()) return;

    spGpsDevice->close();
}

void SerialPortGpsDeviceController::setupGpsDevice() {
    spGpsDevice->setDataBits(QSerialPort::Data8);
    spGpsDevice->setFlowControl(QSerialPort::NoFlowControl);
    spGpsDevice->setParity(QSerialPort::NoParity);
    spGpsDevice->setPortName(DEVICE_PORT_NAME);
    spGpsDevice->setStopBits(QSerialPort::OneStop);
}

bool SerialPortGpsDeviceController::resetGpsDevice() {
    spGpsDevice->setBaudRate(QSerialPort::Baud9600);

    // TOOD: Reset power on pins
    auto is_device_open = spGpsDevice->open(QSerialPort::ReadOnly);

    return is_device_open;
}

void SerialPortGpsDeviceController::handleGpsDeviceReadyRead() {
    // Remove sentences from read buffer
    sentences.clear();
    Nmea::process_input(*spGpsDevice, sentences);

    for (auto& sentence : sentences) {
        // TODO: Looks like we can replace action_if with Boost::Hana
        Utils::Casting::action_if<Nmea::GgaSentence>(sentence, [=](auto sentence) {
            ggaSentence = sentence;
        });

        Utils::Casting::action_if<Nmea::RmcSentence>(sentence, [=](auto sentence) {
            rmcSentence = sentence;
        });

        if (!ggaSentence || !rmcSentence) {
            continue;
        }

        // Skip update if GGA and RMC were not collected at the same time.
        if (ggaSentence->time != rmcSentence->datetime.time()) {
            continue;
        }

        emit notifyGpsDataUpdated(GpsUpdate {
            rmcSentence->datetime,
            rmcSentence->isValid,
            rmcSentence->lat,
            rmcSentence->lon,
        });
    }
}
