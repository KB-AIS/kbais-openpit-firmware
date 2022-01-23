#include "AgtpUsbCommandsReciever.h"

// oss
#include <fmt/core.h>
#include <pipes/operator.hpp>
#include <pipes/push_back.hpp>
#include <pipes/transform.hpp>
#include <plog/Log.h>

#include "Handlers/IAgtpRequestHandler.h"
#include "AgtpCommandsParser.h"

using namespace std::chrono;

using TerminalError = QSerialPort::SerialPortError;

AgtpUsbCommandsReciever::AgtpUsbCommandsReciever(const AgtpRequestsSender& mediator)
    : IAgtpCommandsReciever()
    , mMediator { mediator }
    , mSpUsbDevice { new QSerialPort(this) }
{
    mSubs = rxcpp::composite_subscription();

    constexpr int PEEK_BYTES = 512 * 1024;

    rxqt::from_signal(mSpUsbDevice, &QIODevice::readyRead)
        // TODO: Move to processing pipeline class
        .tap([](auto) {
            PLOGV << "AGTP service recived data to read";
        })
        .map([&](auto) {
            return AgtpRequestsParser::parseFrame((*mSpUsbDevice).read(PEEK_BYTES));
        })
        .tap([](const QVector<AgtpRequest>& commands) {
            PLOGD << fmt::format("AGTP serivce deserialized {} command(s)", commands.size());
        })
        .map([&](const QVector<AgtpRequest>& commands) {
            QVector<AgtpResponse> commandResults;

            commands
                >>= pipes::transform([&](auto command) { return mMediator.handle(command); })
                >>= pipes::push_back(commandResults);

            return commandResults;
        })
        .map(AgtpRequestsParser::createFrame)
        .subscribe(
            mSubs
        ,   [&](const QByteArray& x) {
                const auto bytesWrittenCount = mSpUsbDevice->write(x);
                PLOGD << fmt::format(
                    "AGTP service answered with frame of {} bytes"
                ,   bytesWrittenCount
                );
            }
        ,   [](std::exception_ptr ep){
                PLOGE << fmt::format(
                    "AGTP service got an exception during command handling: {}"
                ,   rxcpp::util::what(ep)
                );
            }
        );

    rxqt::from_signal(mSpUsbDevice, &QSerialPort::errorOccurred)
        .subscribe(
            mSubs
        ,   [&](const TerminalError& x) {
                if (x == TerminalError::NoError) return;

                PLOGE << fmt::format(
                    "AGTP service got an error from terminal device: {}, resetting"
                ,   mSpUsbDevice->errorString().toStdString()
                );

                resetTerminal();
            }
        );
}

AgtpUsbCommandsReciever::~AgtpUsbCommandsReciever() {
    mSubs.unsubscribe();

    delete mSpUsbDevice;
}

void
AgtpUsbCommandsReciever::startProcessing() {
    PLOGI << "AGTP service is starting processing";
    setupTerminal();

    mSpUsbDevice->open(QIODevice::ReadWrite);
}

void
AgtpUsbCommandsReciever::setupTerminal() {
    const QString AGTP_TERMINAL_PATH { QStringLiteral("/dev/ttyGS1") };

    mSpUsbDevice->setPortName(AGTP_TERMINAL_PATH);
    mSpUsbDevice->setBaudRate(9600);
    mSpUsbDevice->setDataBits(QSerialPort::Data8);
}

void
AgtpUsbCommandsReciever::resetTerminal() {
    mSpUsbDevice->close();
    mSpUsbDevice->open(QIODevice::ReadWrite);
}
