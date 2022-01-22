#include "AgtpUsbCommandsReciever.h"

// oss
#include <fmt/core.h>
#include <pipes/operator.hpp>
#include <pipes/push_back.hpp>
#include <pipes/transform.hpp>
#include <plog/Log.h>

#include "AgtpCommandsParser.h"
#include "Handlers/IAgtpCommandHandler.h"

using namespace std::chrono;

using TerminalError = QSerialPort::SerialPortError;

AgtpUsbCommandsReciever::AgtpUsbCommandsReciever(const AgtpCommandsMediator& mediator)
    : QObject()
    , mMediator { mediator }
    , mSpUsbDevice { new QSerialPort(this) }
{
    mSubs = rxcpp::composite_subscription();

    constexpr int PEEK_BYTES = 512 * 1024;

    rxqt::from_signal(mSpUsbDevice, &QIODevice::readyRead)
        .tap([](auto) {
            PLOGV << "AGTP service recived data to read";
        })
        .map([&](auto) {
            return AgtpCommandsParser::parseFrame((*mSpUsbDevice).read(PEEK_BYTES));
        })
        .tap([](const QVector<AgtpCommand>& x) {
            PLOGD << fmt::format("AGTP serivce deserialized {} command(s)", x.size());
        })
        .map([&](const QVector<AgtpCommand>& x) {
            QVector<AgtpCommandResult> z;

            x >>= pipes::transform([&](auto y) { return mMediator.handle(y); })
              >>= pipes::push_back(z);

            return z;
        })
        .map(AgtpCommandsParser::createFrame)
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
                    "AGTP service got an error from terminal device {}, resetting"
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

void AgtpUsbCommandsReciever::startProcessing() {
    PLOGI << "AGTP service is starting processing";
    setupTerminal();

    mSpUsbDevice->open(QIODevice::ReadWrite);
}

void AgtpUsbCommandsReciever::setupTerminal() {
    const QString AGTP_TERMINAL_PATH { QStringLiteral("/dev/ttyGS1") };

    mSpUsbDevice->setPortName(AGTP_TERMINAL_PATH);
    mSpUsbDevice->setBaudRate(9600);
    mSpUsbDevice->setDataBits(QSerialPort::Data8);
}

void AgtpUsbCommandsReciever::resetTerminal() {
    mSpUsbDevice->close();
    mSpUsbDevice->open(QIODevice::ReadWrite);
}
