#ifndef TCPMESSAGESENDERSMANAGER_H
#define TCPMESSAGESENDERSMANAGER_H

// qt
#include <QObject>

#include "Core/Networking/Diagnostic/IRxMessageSendersDiagPub.h"
#include "Core/Networking/MessageSenders/IMessageSendersManager.h"
#include "Core/Networking/MessageSenders/TcpMessageSender.h"
#include "Core/Networking/MessageSenders/TcpMessageSenderConfiguration.h"
#include "core/configuration/app_configuration_manager.h"

class TcpMessageSendersManager
    :   public IMessageSendersManager
    ,   public IRxMessageSendersDiagPub
{
    Q_OBJECT

    using MessageSenderId_t = QString;

    using MessageSenderConfigurations_t = std::map<MessageSenderId_t, TcpMessageSenderConfiguration>;

    using MessageSenderState_t = std::pair<MessageSenderId_t, TcpMessageSenderState>;

    using MessageSenderStates_t = std::map<MessageSenderId_t, TcpMessageSenderState>;

    using MessageSenders_t = std::map<MessageSenderId_t, std::unique_ptr<TcpMessageSender>>;

    QString m_equipment_id;

public:
    explicit TcpMessageSendersManager(const i_app_configuration_publisher& app_configuration_publisher);

    void StartWorkOn(const rxcpp::observe_on_one_worker& coordinator) override;

    rxcpp::observable<MessageSenderDiagInfos_t> GetObservableDiagInfo() const override;

private:
    const i_app_configuration_publisher& app_configuration_publisher_;

    rxcpp::composite_subscription m_subsConfigurationChanged;

    rxcpp::composite_subscription m_subsMessageSenderStatesChanged;

    rxcpp::observable<long> m_obsMessageSednersRestartInterval;

    rxcpp::rxsub::behavior<MessageSenderDiagInfos_t> m_subMessageSenderDiagInfosChanged;

    MessageSenderConfigurations_t m_messageSenderConfigurations;

    MessageSenderStates_t m_messageSenderStates;

    MessageSenders_t m_messageSenders;

    void OnConfigurationChanged(const app_configuration& configuration);

    void OnMessageSendersRestartRequired();

    void OnMessageSenderStateChanged(TcpMessageSenderStateChanged notification);

};

#endif // TCPMESSAGESENDERSMANAGER_H
