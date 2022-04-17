#ifndef LLS_SENSOR_PUBLISHER_H
#define LLS_SENSOR_PUBLISHER_H

// qt
#include <QObject>
#include <QSerialPort>

#include "core/configuration/app_configuration_manager.h"
#include "modules/sensors/serials/lls/lls_formatter_omnicomm.h"

using LlsDeviceAddress_t = quint8;

using LlsDeviceCombinedData_t = std::map<LlsDeviceAddress_t, std::optional<lls_reply_readdata>>;

enum LlsDeviceError {
    NoData
,   FailedToSendRequest
,   PartialData
,   InvalidData
};

struct lls_sensor_message {
    LlsDeviceCombinedData_t data;
};

struct lls_sensor_health {
    bool isConnected;

    std::optional<LlsDeviceError> error;
};

class i_lls_sensor_publisher {

public:
    virtual const rxcpp::observable<lls_sensor_message> get_observable() const = 0;

};

class lls_sensor_publisher : public QObject, public i_lls_sensor_publisher {
    Q_OBJECT

    struct LlsReplyReadDataResult {
        int expectedReplies { 0 };

        std::vector<lls_reply_readdata> replies { };
    };

public:
    lls_sensor_publisher(app_configuration_manager& configurationPublisher);

    ~lls_sensor_publisher();

    void start_work_on(const rxcpp::observe_on_one_worker& coord);

    const rxcpp::observable<lls_sensor_message> get_observable() const override;

    const rxcpp::observable<lls_sensor_health> GetObservableHealthStatus() const;

private:
    app_configuration_manager& m_configurationPublisher;

    QSerialPort m_spLlsDevice;

    LlsReplyReadDataResult m_decodeReplyResult;

    rxcpp::composite_subscription m_subsBag;

    rxcpp::rxsub::behavior<lls_sensor_message> m_subLlsDeviceMessage;

    rxcpp::rxsub::behavior<lls_sensor_health> m_subLlsDeviceHealth;

    void ConfigConnection();

    void RequestSingleRead();

    void PublishLlsDeviceMessage();

    void PublishLlsDeviceHealth(std::optional<LlsDeviceError> error = std::nullopt);

    void HandleReadyRead();

};

#endif // LLS_SENSOR_PUBLISHER_H
