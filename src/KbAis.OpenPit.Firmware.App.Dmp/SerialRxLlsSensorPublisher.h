#ifndef SERIALRXLLSSENSORPUBLISHER_H
#define SERIALRXLLSSENSORPUBLISHER_H

// qt
#include <QObject>
#include <QSerialPort>

#include "IRxConfigurationChangePublisher.h"
#include "OmnicommLlsProtocolFomratter.h"
#include "RxQt.h"

using LlsDeviceAddress_t = quint8;

using LlsDeviceCombinedData_t = std::map<LlsDeviceAddress_t, std::optional<LlsReplyReadData>>;

enum LlsDeviceError {
    NoData
,   FailedToSendRequest
,   PartialData
,   InvalidData
};

struct LlsDeviceMessage {
    LlsDeviceCombinedData_t data;
};

struct LlsDeviceHealth {
    bool isConnected;

    std::optional<LlsDeviceError> error;
};

class IRxLlsDeviceMessagePublisher {

public:
    virtual const rxcpp::observable<LlsDeviceMessage> GetObservableMessage() const = 0;

};

class SerialRxLlsSensorPublisher
    :   public QObject
    ,   public IRxLlsDeviceMessagePublisher
{
    Q_OBJECT

    struct LlsReplyReadDataResult {
        int expectedReplies { 0 };

        std::vector<LlsReplyReadData> replies { };
    };

public:
    SerialRxLlsSensorPublisher(IRxConfigurationChangePublisher& configurationPublisher);

    ~SerialRxLlsSensorPublisher();

    void StartPublishOn(const rxcpp::observe_on_one_worker& coordinator);

    const rxcpp::observable<LlsDeviceMessage> GetObservableMessage() const override;

    const rxcpp::observable<LlsDeviceHealth> GetObservableHealthStatus() const;

private:
    IRxConfigurationChangePublisher& m_configurationPublisher;

    QSerialPort m_spLlsDevice;

    LlsReplyReadDataResult m_decodeReplyResult;

    rxcpp::composite_subscription m_subsBag;

    rxcpp::rxsub::behavior<LlsDeviceMessage> m_subLlsDeviceMessage;

    rxcpp::rxsub::behavior<LlsDeviceHealth> m_subLlsDeviceHealth;

    void ConfigConnection();

    void RequestSingleRead();

    void PublishLlsDeviceMessage();

    void PublishLlsDeviceHealth(std::optional<LlsDeviceError> error = std::nullopt);

    void HandleReadyRead();

};

#endif // SERIALRXLLSSENSORPUBLISHER_H
