#ifndef TCPMESSAGESENDERSMANAGER_H
#define TCPMESSAGESENDERSMANAGER_H

// qt
#include <QObject>
// oss
#include <rxcpp/rx.hpp>

// Core.Configuration
#include "IRxConfigurationChangePublisher.h"

#include "IMessageSendersManager.h"
#include "IRxMessageSendersStatusPublisher.h"
#include "TcpMessageSender.h"

class TcpMessageSendersManager
    :   public QObject
    ,   public IMessageSendersManager
    ,   public IRxMessageSendersStatusPublisher
{
    Q_OBJECT

    using MessageSenderConfigurations_t = std::map<QString, MessageSenderConfiguration>;

public:
    explicit TcpMessageSendersManager(
        IRxConfigurationChangePublisher& configurationPublisher
    );

    void start_work_on(rxcpp::observe_on_one_worker& scheduler);

    rxcpp::observable<QVector<MessageSenderStatus>> get_diag_observable() const override;

private:
    IRxConfigurationChangePublisher& m_configuration_publisher;

    rxcpp::composite_subscription m_subscriptions;

    std::map<QString, std::unique_ptr<TcpMessageSender>> m_message_senders;

    void on_configuration_changed(AppConfiguration configuration);

    void restart_message_senders();

};

#endif // TCPMESSAGESENDERSMANAGER_H
