#ifndef TCPMESSAGESENDERSMANAGER_H
#define TCPMESSAGESENDERSMANAGER_H

// qt
#include <QObject>
// oss
#include <rxcpp/rx.hpp>

// Core.Configuration
#include "IRxConfigurationChangePublisher.h"

#include "Networking/Confguration/TcpMessageSenderConfiguration.h"
#include "Networking/Diagnosing/IRxMessageSendersDiagPub.h"
#include "Networking/MessageSenders/IMessageSendersManager.h"
#include "Networking/MessageSenders/TcpMessageSender.h"

class TcpMessageSendersManager
    :   public QObject
    ,   public IMessageSendersManager
    ,   public IRxMessageSendersDiagPub
{
    Q_OBJECT

    using MessageSenderId_t = QString;

    using MessageSenderConfigurations_t = std::map<MessageSenderId_t, TcpMessageSenderConfiguration>;

    using MessageSenderState_t = std::pair<MessageSenderId_t, TcpMessageSenderState>;

    using MessageSenderStates_t = std::map<MessageSenderId_t, TcpMessageSenderState>;

    using MessageSenders_t = std::map<MessageSenderId_t, std::unique_ptr<TcpMessageSender>>;

public:
    explicit TcpMessageSendersManager(
        IRxConfigurationChangePublisher& configuration_publisher
    );

    Q_SLOT void start_work_on(rxcpp::observe_on_one_worker& coordinator);

    rxcpp::observable<MessageSenderDiagInfos_t> get_diag_observable() const override;

private:
    IRxConfigurationChangePublisher& m_configuration_publisher;

    rxcpp::composite_subscription m_subscriptions;

    rxcpp::composite_subscription m_status_subscriptions;

    rxcpp::rxsub::behavior<MessageSenderDiagInfos_t> m_subject;

    MessageSenderConfigurations_t m_message_sender_configurations;

    MessageSenderStates_t m_message_sender_states;

    MessageSenders_t m_message_senders;

    void on_configuration_changed(AppConfiguration configuration);

    void on_message_senders_restart_required();

    void on_message_sender_state_changed(TcpMessageSenderStateChanged);

};

#endif // TCPMESSAGESENDERSMANAGER_H
