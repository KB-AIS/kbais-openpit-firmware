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

struct LlsDeviceMessage {
     LlsDeviceCombinedData_t data;
};

struct LlsDeviceHelth {

};

class IRxLlsDeviceMessagePublisher {

public:
    virtual const rxcpp::observable<LlsDeviceMessage> GetObservableLlsDeviceMessage() const = 0;

};

class IRxLlsDeviceHealthObserver {

};

class SerialRxLlsSensorPublisher
    :   public QObject
    ,   public IRxLlsDeviceMessagePublisher
    ,   public IRxLlsDeviceHealthObserver
{
    Q_OBJECT

    using DecodeError_t = std::optional<OmnicommLlsProtocolFomratter::DecodeReplyError>;

public:
    SerialRxLlsSensorPublisher(IRxConfigurationChangePublisher& configurationPublisher);

    ~SerialRxLlsSensorPublisher();

    void StartPublishOn(const rxcpp::observe_on_one_worker& coordinator);

    const rxcpp::observable<LlsDeviceMessage> GetObservableLlsDeviceMessage() const override;

    rxcpp::observable<LlsDeviceHelth> GetObservableHelthCheck() const;

private:
    struct LlsReplyReadDataResult {
        int expectedRepliesAmount { 0 };

        std::vector<LlsReplyReadData> recivedReplies { };
    };

    IRxConfigurationChangePublisher& m_configurationPublisher;

    QSerialPort m_spLlsDevice;

    LlsReplyReadDataResult m_decodeReplyResult;

    rxcpp::composite_subscription m_subsBag;

    rxcpp::rxsub::behavior<LlsDeviceMessage> m_subLlsDeviceMessage;

    rxcpp::rxsub::behavior<LlsDeviceHelth> m_subLlsDeviceDiagInfo;

    void ConfigConnection(int idx);

    void RequestSingleRead();

    void PublishLlsDeviceMessage();

    void PublishLlsDeviceDiagInfo();

    void HandleReadyRead();

};

#endif // SERIALRXLLSSENSORPUBLISHER_H
