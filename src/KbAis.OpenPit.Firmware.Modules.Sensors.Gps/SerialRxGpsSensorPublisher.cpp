#include "SerialRxGpsSensorPublisher.h"

// std
#include <typeinfo>
#include <functional>
// qt
#include <QtGlobal>

#include "Casting.h"
#include "GpioUtils.h"

// !Q: where device name comes from?
static const QString DEVICE_PORT_NAME { "/dev/ttyS5" };

gps_sensor_publisher::gps_sensor_publisher()
    : serialGpsSensor { this }
    , subject { GpsMessage {} }
    , lastGgaSentence { std::make_shared<GgaSentence>() }
    , lastRmcSentence { std::make_shared<RmcSentence>() }
{
    connect(
        &serialGpsSensor, &QSerialPort::readyRead,
        this, &gps_sensor_publisher::OnGpsSensorReadyRead
    );

    GpioUtils::export_pin(168);
    GpioUtils::setup_dir(168, GpioUtils::PinDirection::Out);

    SetupGpsDevice();
    ResetGpsDevice();
}

gps_sensor_publisher::~gps_sensor_publisher() {
    if (!serialGpsSensor.isOpen()) return;

    serialGpsSensor.close();
}

const rxcpp::observable<GpsMessage>
gps_sensor_publisher::get_observable() const {
    return subject.get_observable();
}

void
gps_sensor_publisher::SetupGpsDevice() {
    serialGpsSensor.setBaudRate(QSerialPort::Baud9600);
    serialGpsSensor.setDataBits(QSerialPort::Data8);
    serialGpsSensor.setFlowControl(QSerialPort::NoFlowControl);
    serialGpsSensor.setParity(QSerialPort::NoParity);
    serialGpsSensor.setPortName(DEVICE_PORT_NAME);
    serialGpsSensor.setStopBits(QSerialPort::OneStop);
}

bool gps_sensor_publisher::ResetGpsDevice() {
    GpioUtils::setup_val(168, 0);

    // TODO: Use non-blocking way
    struct timespec req_time { 2, 0 }, rem_time;
    nanosleep(&req_time, &rem_time);

    GpioUtils::setup_val(168, 1);

    auto opened = serialGpsSensor.open(QSerialPort::ReadOnly);

    return opened;
}

void
gps_sensor_publisher::OnGpsSensorReadyRead() {
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

        if (!NeedPublishGpsMessage()) continue;

        subject.get_subscriber().on_next(MapGpsMessage());
    }
}

bool
gps_sensor_publisher::NeedPublishGpsMessage() const {
    // Check if sentences from same receive frame
    if (lastGgaSentence->time != lastRmcSentence->datetime.time()) {
        return false;
    }

    return true;
}

GpsMessage
gps_sensor_publisher::MapGpsMessage() const {
    constexpr double SPD_KNOTS_TO_KHM_RATIO { 1.852 };

    return GpsMessage {
        lastRmcSentence->datetime,
        lastRmcSentence->isValid,
        lastRmcSentence->latitude,
        lastRmcSentence->longitude,
        lastRmcSentence->speed * SPD_KNOTS_TO_KHM_RATIO,
        lastGgaSentence->gpsQauality,
        lastGgaSentence->satellites
    };
}
