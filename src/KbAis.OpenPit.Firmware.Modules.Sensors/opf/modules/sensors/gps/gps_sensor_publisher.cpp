#include "gps_sensor_publisher.h"

// std
#include <functional>
// qt
#include <QtCore/qglobal.h>

#include "Casting.h"
#include "gpio_utils.h"

const QString GPS_SENSOR_PATH { "/dev/ttyS5" };

gps_sensor_publisher::gps_sensor_publisher()
    :   serialGpsSensor { this }
    ,   subject { gps_sensor_message {} }
    ,   lastGgaSentence { std::make_shared<GgaSentence>() }
    ,   lastRmcSentence { std::make_shared<RmcSentence>() }
{
    QObject::connect(&serialGpsSensor, &QSerialPort::readyRead, this, &gps_sensor_publisher::handle_ready_read);

    gpio_utils::export_pin(168);
    gpio_utils::setup_dir(168, gpio_utils::PinDirection::Out);

    SetupGpsDevice();
    ResetGpsDevice();
}

gps_sensor_publisher::~gps_sensor_publisher() {
    if (!serialGpsSensor.isOpen()) return;

    serialGpsSensor.close();
}

const rxcpp::observable<gps_sensor_message> gps_sensor_publisher::get_observable() const {
    return subject.get_observable();
}

void gps_sensor_publisher::SetupGpsDevice() {
    serialGpsSensor.setBaudRate(QSerialPort::Baud9600);
    serialGpsSensor.setDataBits(QSerialPort::Data8);
    serialGpsSensor.setFlowControl(QSerialPort::NoFlowControl);
    serialGpsSensor.setParity(QSerialPort::NoParity);
    serialGpsSensor.setPortName(GPS_SENSOR_PATH);
    serialGpsSensor.setStopBits(QSerialPort::OneStop);
}

bool gps_sensor_publisher::ResetGpsDevice() {
    gpio_utils::setup_val(168, 0);

    // TODO: Use non-blocking way
    struct timespec req_time { 2, 0 }, rem_time;
    nanosleep(&req_time, &rem_time);

    gpio_utils::setup_val(168, 1);

    auto opened = serialGpsSensor.open(QSerialPort::ReadOnly);

    return opened;
}

void gps_sensor_publisher::handle_ready_read() {
    // Remove sentences from read buffer
    nmeaSentences.clear();
    processDeviceRead(serialGpsSensor, nmeaSentences);

    for (const auto& sentence : nmeaSentences) {
        // TODO: Looks like we can replace action_if with Boost::Hana
        Utils::Casting::action_if<GgaSentence>(sentence, [=](auto sentence) mutable {
            lastGgaSentence = sentence;
        });

        Utils::Casting::action_if<RmcSentence>(sentence, [=](auto sentence) mutable {
            lastRmcSentence = sentence;
        });

        if (!NeedPublishGpsMessage()) continue;

        subject.get_subscriber().on_next(map_gps_senosr_message());
    }
}

bool gps_sensor_publisher::NeedPublishGpsMessage() const {
    // Check if sentences from same receive frame
    if (lastGgaSentence->time != lastRmcSentence->datetime.time()) {
        return false;
    }

    return true;
}

gps_sensor_message gps_sensor_publisher::map_gps_senosr_message() const {
    constexpr double SPD_KNOTS_TO_KHM_RATIO { 1.852 };

    return gps_sensor_message {
        lastRmcSentence->datetime,
        lastRmcSentence->isValid,
        lastRmcSentence->latitude,
        lastRmcSentence->longitude,
        lastRmcSentence->speed * SPD_KNOTS_TO_KHM_RATIO,
        lastGgaSentence->gpsQauality,
        lastGgaSentence->satellites
    };
}
