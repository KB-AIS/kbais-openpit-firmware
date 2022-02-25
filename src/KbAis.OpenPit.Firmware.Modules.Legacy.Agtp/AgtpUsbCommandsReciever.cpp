#include "AgtpUsbCommandsReciever.h"

// oss
#include <fmt/format.h>
#include <plog/Log.h>
#include <range/v3/all.hpp>

#include "Handlers/IAgtpRequestHandler.h"
#include "AgtpProtocolPareser.h"

using namespace std::chrono;

using TerminalError = QSerialPort::SerialPortError;

AgtpUsbRequestsReciever::AgtpUsbRequestsReciever(const IAgtpRequestsMediator& mediator)
    :   IAgtpRequetsReciever()
    ,   m_mediator { mediator }
    ,   m_spUsbDevice { new QSerialPort(this) }
{
    m_subscriptions = rxcpp::composite_subscription();

    constexpr int PEEK_BYTES = 512 * 1024;

    rxqt::from_signal(m_spUsbDevice, &QIODevice::readyRead)
        .subscribe(
            m_subscriptions
        ,   [&](auto) {
                PLOGV << "recived data to read";

                const auto parse_result =
                    AgtpProtocolParser::parseRequest((*m_spUsbDevice).peek(PEEK_BYTES));

                if (!parse_result) return;

                m_spUsbDevice->read(PEEK_BYTES);

                PLOGD << fmt::format("deserialized {} request(s)", parse_result->size());

                const auto& requests = parse_result.value();

                auto responses = AgtpProtocolParser::createResponse(
                    requests
                |   ranges::views::transform([&](const AgtpRequest& x) { return m_mediator.handle(x); })
                |   ranges::to<std::vector<AgtpResponse>>()
                );

                const auto bytesWrittenCount = m_spUsbDevice->write(responses);
                PLOGD << fmt::format("answered with frame of {} bytes", bytesWrittenCount);
            }
        ,   [](std::exception_ptr ep){
                PLOGE << fmt::format("got an exception during command: {}", rxcpp::util::what(ep));
            }
        );

    rxqt::from_signal(m_spUsbDevice, &QSerialPort::errorOccurred)
        .subscribe(
            m_subscriptions
        ,   [&](const TerminalError& x) {
                if (x == TerminalError::NoError) return;

                PLOGE << fmt::format("got a terminal error : {}", m_spUsbDevice->errorString().toStdString());

                ResetTerminal();
            }
        );
}

AgtpUsbRequestsReciever::~AgtpUsbRequestsReciever() {
    m_subscriptions.unsubscribe();

    delete m_spUsbDevice;
}

void
AgtpUsbRequestsReciever::Start() {
    PLOGI << "starting processing";
    SetupTerminal();

    m_spUsbDevice->open(QIODevice::ReadWrite);
}

void
AgtpUsbRequestsReciever::SetupTerminal() {
    const QString AGTP_TERMINAL_PATH { QStringLiteral("/dev/ttyGS1") };

    m_spUsbDevice->setPortName(AGTP_TERMINAL_PATH);
    m_spUsbDevice->setBaudRate(9600);
    m_spUsbDevice->setDataBits(QSerialPort::Data8);
}

void
AgtpUsbRequestsReciever::ResetTerminal() {
    m_spUsbDevice->close();
    m_spUsbDevice->open(QIODevice::ReadWrite);
}
