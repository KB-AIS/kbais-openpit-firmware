#include "AgtpUsbCommandsReciever.h"

// oss
#include <fmt/core.h>
#include <pipes/operator.hpp>
#include <pipes/push_back.hpp>
#include <pipes/transform.hpp>
#include <plog/Log.h>

#include "Handlers/IAgtpRequestHandler.h"
#include "AgtpProtocolPareser.h"

using namespace std::chrono;

using TerminalError = QSerialPort::SerialPortError;

AgtpUsbRequestsReciever::AgtpUsbRequestsReciever(const IAgtpRequestsMediator& mediator)
    :   IAgtpRequetsReciever()
    ,   mMediator { mediator }
    ,   mSpUsbDevice { new QSerialPort(this) }
{
    mSubs = rxcpp::composite_subscription();

    constexpr int PEEK_BYTES = 512 * 1024;

    rxqt::from_signal(mSpUsbDevice, &QIODevice::readyRead)
        .map([&](auto) {
            PLOGV << "AGTP service recived data to read";

            const auto requests =
                AgtpProtocolParser::parseRequest((*mSpUsbDevice).read(PEEK_BYTES));

            PLOGD << fmt::format("AGTP serivce deserialized {} request(s)", requests.size());
            QVector<AgtpResponse> responses;

            requests
                >>= pipes::transform([&](auto command) { return mMediator.handle(command); })
                >>= pipes::push_back(responses);

            return AgtpProtocolParser::createResponse(responses);
        })
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

AgtpUsbRequestsReciever::~AgtpUsbRequestsReciever() {
    mSubs.unsubscribe();

    delete mSpUsbDevice;
}

void
AgtpUsbRequestsReciever::start() {
    PLOGI << "AGTP service is starting processing";
    setupTerminal();

    mSpUsbDevice->open(QIODevice::ReadWrite);
}

void
AgtpUsbRequestsReciever::setupTerminal() {
    const QString AGTP_TERMINAL_PATH { QStringLiteral("/dev/ttyGS1") };

    mSpUsbDevice->setPortName(AGTP_TERMINAL_PATH);
    mSpUsbDevice->setBaudRate(9600);
    mSpUsbDevice->setDataBits(QSerialPort::Data8);
}

void
AgtpUsbRequestsReciever::resetTerminal() {
    mSpUsbDevice->close();
    mSpUsbDevice->open(QIODevice::ReadWrite);
}
