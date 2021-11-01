#include "gps_sensor_thread.h"

// plog
#include <plog/Log.h>
// qdef
#include <QDeferred>
#include <QLambdaThreadWorker>

using namespace Sensors::Gps;

const char SENTENCE_START { '$' };

const char SENTENCE_FINISH[] { '\r', '\n' };

const int SENTENCE_MAX_LENGTH { 82 };

const int PEEK_SIZE { 1024 };

gps_sensor_thread::gps_sensor_thread(QObject *parent) : QThread(parent) { }

gps_sensor_thread::~gps_sensor_thread() {
    // TODO: close m_gps_device
    m_gps_device->close();

    delete m_gps_device;
}

void gps_sensor_thread::run() {
    auto setup_gps_device = [](QSerialPort *device) {
        // !QUESTION: where device name comes from?
        const QString device_port_name = "/dev/ttyS5";

        device->setDataBits(QSerialPort::Data8);
        device->setFlowControl(QSerialPort::NoFlowControl);
        device->setParity(QSerialPort::NoParity);
        device->setPortName(device_port_name);
        device->setStopBits(QSerialPort::OneStop);
    };

    auto reset_gps_device = [](QSerialPort *device) {
        device->setBaudRate(QSerialPort::Baud9600);

        // TOOD: Reset power on pins
        auto isDeviceOpen = device->open(QSerialPort::ReadOnly);

        return isDeviceOpen;
    };

    bool is_frame_open = false;

    m_gps_device = new QSerialPort();

    connect(
        m_gps_device, &QSerialPort::readyRead,
        this, [&] {
            //PLOGV << "Reading from GPS device";

            const auto data_to_scan = m_gps_device->peek(PEEK_SIZE);

            /**
             * 1. Sentence begining ($)
             * 2. Read command name (to the first ',')
             * 3. Read data up to (*, every fields are delimetred by ',')
             * 4. Read sentence end ('\r\n')
             */
            auto sentence_start_pos = data_to_scan.indexOf(SENTENCE_START);
            if (sentence_start_pos == -1) {
                m_gps_device->read(PEEK_SIZE);

                return;
            }

            auto sentence_finish_pos = data_to_scan.indexOf(SENTENCE_FINISH);
            if (sentence_finish_pos == -1) {
                return;
            }

            PLOGD << data_to_scan.mid(sentence_start_pos, sentence_finish_pos - sentence_start_pos);
            m_gps_device->read(sentence_finish_pos + 1);
        }
    );

    PLOGI << "Setup GPS device in thread";
    setup_gps_device(m_gps_device);

    PLOGI << "Reset GPS device";
    reset_gps_device(m_gps_device);

    forever {
        emit update_gps_data_signal();

        auto exit_code = exec();
    }
}
