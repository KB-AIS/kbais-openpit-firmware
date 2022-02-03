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

    using MessageSenderId_t = QString;

    using MessageSenderConfigurations_t = std::map<MessageSenderId_t, MessageSenderConfiguration>;

    using MessageSenderStates_t = std::map<MessageSenderId_t, TcpMessageSenderState>;

    using MessageSenders_t = std::map<MessageSenderId_t, std::unique_ptr<TcpMessageSender>>;

public:
    explicit TcpMessageSendersManager(
        IRxConfigurationChangePublisher& configurationPublisher
    );

    void start_work_on(rxcpp::observe_on_one_worker& scheduler);

    rxcpp::observable<QVector<MessageSenderStatus>> get_diag_observable() const override;

private:
    IRxConfigurationChangePublisher& m_configuration_publisher;

    rxcpp::composite_subscription m_main_subs;

    rxcpp::composite_subscription m_message_sender_status_changed_subs;

    MessageSenderConfigurations_t m_message_sender_configurations;

    MessageSenderStates_t m_message_sender_states;

    MessageSenders_t m_message_senders;

    void on_configuration_changed(AppConfiguration configuration);

    void on_message_senders_restart_required();

};

#endif // TCPMESSAGESENDERSMANAGER_H
