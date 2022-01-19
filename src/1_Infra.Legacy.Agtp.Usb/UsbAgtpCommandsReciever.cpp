#include "UsbAgtpCommandsReciever.h"

// oss
#include <fmt/core.h>
#include <plog/Log.h>

#include "AgtpProtocolSerializer.h"
#include "Handlers/FetchDeviceInfoCommandHandler.h"
#include "Handlers/FetchFullDeviceInfoCommandHandler.h"
#include "Handlers/FetchSupportModulesCommandHandler.h"
#include "Handlers/IAgtpCommandHandler.h"

using namespace std::chrono;

using SerialPortError = QSerialPort::SerialPortError;

const QString AGTP_TERMINAL_PATH { QStringLiteral("/dev/ttyGS1") };

constexpr int PEEK_SIZE = 512 * 1024;

UsbAgtpCommandsReciever::UsbAgtpCommandsReciever()
    : QObject()
    , terminal { new QSerialPort(this) }
{
    subs = rxcpp::composite_subscription();

    rxqt::from_signal(terminal, &QIODevice::readyRead).subscribe(
        subs,
        [&](auto) {
            PLOGD << "AGTP service recived data to read";

            const auto commands = AgtpProtocolSerializer::deserialize((*terminal).read(PEEK_SIZE));

            QVector<AgtpCommandResult> commandResults;
            for (auto command : commands) {
                PLOGD << command.command;
                // TODO: Move to mediator
                if (command.command == "DEVICEINFO=?;") {
                    commandResults.push_back(FetchDeviceInfoCommandHandler{}.handle(command));
                } else if (command.command == "SUPPORT_MODULES=?;") {
                    commandResults.push_back(FetchSupportModulesCommandHandler{}.handle(command));
                } else if (command.command == "INFO:?") {
                    commandResults.push_back(FetchFullDeviceInfoCommandHandler{}.handle(command));
                }
            }

            terminal->write(AgtpProtocolSerializer::serialize(commandResults));
        },
        [] { PLOGD << "AGTP service completed to read"; }
    );

    rxqt::from_signal(terminal, &QSerialPort::errorOccurred).subscribe(
        subs,
        [&](SerialPortError error) {
            if (error == QSerialPort::NoError) return;
            resetTerminal();
        }
    );
}

UsbAgtpCommandsReciever::~UsbAgtpCommandsReciever() {
    subs.unsubscribe();

    delete terminal;
}

void
UsbAgtpCommandsReciever::startProcessing() {
    PLOGI << "AGTP service is starting processing";
    setupTerminal();

    terminal->open(QIODevice::ReadWrite);
}

void
UsbAgtpCommandsReciever::setupTerminal() {
    terminal->setPortName(AGTP_TERMINAL_PATH);
    terminal->setBaudRate(9600);
    terminal->setDataBits(QSerialPort::Data8);
}

void
UsbAgtpCommandsReciever::resetTerminal() {
    terminal->close();
    terminal->open(QIODevice::ReadWrite);
}
