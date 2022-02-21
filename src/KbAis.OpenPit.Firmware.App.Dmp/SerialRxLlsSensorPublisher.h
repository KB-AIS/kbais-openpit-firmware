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

using LlsDeviceData_t = std::vector<nonstd::expected<LlsReplyReadData, LlsMessageError>>;

struct LlsDeviceMessage {
     LlsDeviceData_t data;
};

struct LlsDeviceDiagInfo {

};

class SerialRxLlsSensorPublisher : public QObject {
    Q_OBJECT

    using DecodeError_t = std::optional<OmnicommLlsProtocolFomratter::DecodeReplyError>;

public:
    SerialRxLlsSensorPublisher();

    ~SerialRxLlsSensorPublisher();

    void StartPublishOn(const rxcpp::observe_on_one_worker& coordinator);

    rxcpp::observable<LlsDeviceMessage> GetObservableLlsDeviceMessage() const;

    rxcpp::observable<LlsDeviceDiagInfo> GetObservableLlsDeviceDiagInfo() const;

private:
    struct LlsReplyReadDataResult {
        int expectedRepliesAmount { 0 };

        std::vector<LlsReplyReadData> recivedReplies { };
    };

    QSerialPort m_spLlsDevice;

    LlsReplyReadDataResult m_decodeReplyResult;

    rxcpp::composite_subscription m_subsBag;

    rxcpp::rxsub::behavior<LlsDeviceMessage> m_subLlsDeviceMessage;

    rxcpp::rxsub::behavior<LlsDeviceDiagInfo> m_subLlsDeviceDiagInfo;

    void ConfigConnection();

    void RequestSingleRead();

    void PublishLlsDeviceMessage();

    void PublishLlsDeviceDiagInfo();

    void HandleReadyRead();

};

#endif // SERIALRXLLSSENSORPUBLISHER_H
