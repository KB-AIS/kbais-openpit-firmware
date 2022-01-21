#include "AgtpUsbCommandsReciever.h"

// oss
#include <fmt/core.h>
#include <plog/Log.h>

#include "AgtpCommandsParser.h"
#include "Handlers/IAgtpCommandHandler.h"

using namespace std::chrono;

using SerialPortError = QSerialPort::SerialPortError;

const QString AGTP_TERMINAL_PATH { QStringLiteral("/dev/ttyGS1") };

constexpr int PEEK_BYTES = 512 * 1024;

AgtpUsbCommandsReciever::AgtpUsbCommandsReciever(const AgtpCommandsMediator& mediator)
    : QObject()
    , mMediator { mediator }
    , mSpUsbDevice { new QSerialPort(this) }
{
    mSubs = rxcpp::composite_subscription();

    rxqt::from_signal(mSpUsbDevice, &QIODevice::readyRead).subscribe(mSubs, [&](auto) {
        PLOGV << "AGTP service recived data to read";

        const auto commands = AgtpCommandsParser::parseFrame((*mSpUsbDevice).read(PEEK_BYTES));

        PLOGV << fmt::format("AGTP serivce deserialized {} command(s)", commands.size());

        QVector<AgtpCommandResult> commandResults;
        for (auto&& command : commands) {
            PLOGD << fmt::format("AGTP service is handling {}", command.command.toStdString());

            commandResults.append(mMediator.handle(command));
        }

        mSpUsbDevice->write(AgtpCommandsParser::createFrame(commandResults));
    });

    rxqt::from_signal(mSpUsbDevice, &QSerialPort::errorOccurred).subscribe(
        mSubs,
        [&](SerialPortError error) {
            if (error == QSerialPort::NoError) return;
            resetTerminal();
        }
    );
}

AgtpUsbCommandsReciever::~AgtpUsbCommandsReciever() {
    mSubs.unsubscribe();

    delete mSpUsbDevice;
}

void AgtpUsbCommandsReciever::startProcessing() {
    PLOGI << "AGTP service is starting processing";
    setupTerminal();

    mSpUsbDevice->open(QIODevice::ReadWrite);
}

void AgtpUsbCommandsReciever::setupTerminal() {
    mSpUsbDevice->setPortName(AGTP_TERMINAL_PATH);
    mSpUsbDevice->setBaudRate(9600);
    mSpUsbDevice->setDataBits(QSerialPort::Data8);
}

void AgtpUsbCommandsReciever::resetTerminal() {
    mSpUsbDevice->close();
    mSpUsbDevice->open(QIODevice::ReadWrite);
}
