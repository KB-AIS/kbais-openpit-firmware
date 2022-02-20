#ifndef SERIALRXLLSSENSORPUBLISHER_H
#define SERIALRXLLSSENSORPUBLISHER_H

// qt
#include <QObject>
#include <QSerialPort>

#include "OmnicommLlsProtocolFomratter.h"
#include "RxQt.h"

enum class LlsMessageError {
    LlsDeviceReturnNoData
};

using LlsDeviceData_t =
    std::vector<nonstd::expected<LlsReplyReadData, LlsMessageError>>;

struct LlsDeviceMessage {
     LlsDeviceData_t data;
};

struct LlsDeviceDiagInfo {

};

class SerialRxLlsSensorPublisher : public QObject {
    Q_OBJECT

    using DecodeError_t =
        std::optional<OmnicommLlsProtocolFomratter::DecodeReplyError>;

public:
    SerialRxLlsSensorPublisher();

    ~SerialRxLlsSensorPublisher();

    void StartWorkingOn();

    rxcpp::observable<LlsDeviceMessage> GetObservableLlsDeviceMessage() const;

    rxcpp::observable<LlsDeviceDiagInfo> GetObservableLlsDeviceDiagInfo() const;

private:
    struct LlsReplyReadDataResult {
        int expectedLlsReplies { 0 };

        std::vector<LlsReplyReadData> llsReplies { };
    };

    QSerialPort m_spLlsDevice;

    LlsReplyReadDataResult m_decodeReplyResult;

    rxcpp::composite_subscription m_subsBag;

    rxcpp::rxsub::behavior<LlsDeviceMessage> m_subLlsDeviceMessage;

    rxcpp::rxsub::behavior<LlsDeviceDiagInfo> m_subLlsDeviceDiagInfo;

    void ConfigLlsDeviceConnection();

    void SendLlsRequestSingleRead();

    void PublishLlsDeviceMessage();

    void PublishLlsDeviceDiagInfo();

    void HandleLlsReply();

};

#endif // SERIALRXLLSSENSORPUBLISHER_H
