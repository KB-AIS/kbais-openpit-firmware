#include "LegacyConfigService.h"

// oss
#include <fmt/core.h>
#include <plog/Log.h>

using namespace std::chrono;

using SerialPortError = QSerialPort::SerialPortError;

const QString AGTP_TERMINAL_PATH { QStringLiteral("/dev/ttyGS1") };

LegacyConfigService::LegacyConfigService()
    : QObject()
    , terminal { new QSerialPort(this) }
{
    subs = rxcpp::composite_subscription();

    setupTerminal();

    rxqt::from_signal(terminal, &QIODevice::readyRead).subscribe(
        subs,
        [&](auto) {
            PLOGD << "AGTP service recived data to read";

            // TODO: Process AGTP protocol
            auto buffer = terminal->read(512 * 1024);

            constexpr int MIN_FRAME_LENGTH = 5;
            constexpr int AGTP_PREAMBLE = 0x3E;
            constexpr int AGTP_VERSION2 = 0x02;

            if (buffer.size() <= MIN_FRAME_LENGTH || buffer.at(0) != AGTP_PREAMBLE) return;

            const auto frameLength = *reinterpret_cast<quint32*>(buffer.data() + 1);
            PLOGD << fmt::format("frameLength {}", frameLength);

            if (buffer.size() < frameLength) return;

            if (buffer.at(5) != AGTP_VERSION2) return;

            auto unscannedBytes = frameLength - 12;

            auto contentBytes = buffer.mid(10, unscannedBytes);
            auto contentBytesPtr = contentBytes.data();

            unsigned int bytesScaned { 0u };

            while (unscannedBytes > 0) {
                auto packetType   = contentBytes.at(0);
                PLOGD << "Type: " << static_cast<int>(packetType);
                auto packetLength = *reinterpret_cast<quint32*>(contentBytesPtr + 1);
                auto packetId     = *reinterpret_cast<quint16*>(contentBytesPtr + 5);
                auto packetCrc    = *reinterpret_cast<quint16*>(contentBytesPtr + packetLength - 2);

                auto command = QString::fromUtf8(reinterpret_cast<char*>(contentBytesPtr + 7), packetLength - 9);

                PLOGD << "Recived command:" << command;

                // json
                if (packetType == 125) {

                }
                // command
                if (packetType == 126) {

                }
            }
        },
        [] {
            PLOGD << "AGTP service completed to read";
        }
    );

    rxqt::from_signal(terminal, &QSerialPort::errorOccurred).subscribe(
        subs,
        [&](SerialPortError error) {
            PLOGE << fmt::format(
                "AGTP service got an error during terminal work: {}",
                terminal->errorString().toStdString()
            );

            if (error == QSerialPort::NoError) return;

            resetTerminal();
        }
    );
}

LegacyConfigService::~LegacyConfigService() {
    subs.unsubscribe();

    delete terminal;
}

void
LegacyConfigService::start() {
    terminal->open(QIODevice::ReadWrite);
}

void
LegacyConfigService::setupTerminal() {
    terminal->setPortName(AGTP_TERMINAL_PATH);
    terminal->setBaudRate(9600);
    terminal->setDataBits(QSerialPort::Data8);
}

void
LegacyConfigService::resetTerminal() {
    terminal->close();
    terminal->open(QIODevice::ReadWrite);
}
