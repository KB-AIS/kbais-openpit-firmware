#ifndef TCPMESSAGESENDERSMANAGER_H
#define TCPMESSAGESENDERSMANAGER_H

// qt
#include <QObject>
// oss
#include <rxcpp/rx.hpp>

// Core.Configuration
#include "IRxConfigurationChangePublisher.h"

#include "Core/Networking/Confguration/TcpMessageSenderConfiguration.h"
#include "Core/Networking/Diagnostic/IRxMessageSendersDiagPub.h"
#include "Core/Networking/MessageSenders/IMessageSendersManager.h"
#include "Core/Networking/MessageSenders/TcpMessageSender.h"

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
    explicit TcpMessageSendersManager(IRxConfigurationChangePublisher& configurationPublisher);

    Q_SLOT void StartWorkOn(rxcpp::observe_on_one_worker& coordinator);

    rxcpp::observable<MessageSenderDiagInfos_t> GetObservableDiagInfo() const override;

private:
    IRxConfigurationChangePublisher& m_configurationPublisher;

    rxcpp::composite_subscription m_subsConfigurationChanged;

    rxcpp::composite_subscription m_subsMessageSenderStatesChanged;

    rxcpp::observable<long> m_obsMessageSednersRestartInterval;

    rxcpp::rxsub::behavior<MessageSenderDiagInfos_t> m_subMessageSenderDiagInfosChanged;

    MessageSenderConfigurations_t m_messageSenderConfigurations;

    MessageSenderStates_t m_messageSenderStates;

    MessageSenders_t m_messageSenders;

    void OnConfigurationChanged(AppConfiguration configuration);

    void OnMessageSendersRestartRequired();

    void OnMessageSenderStateChanged(TcpMessageSenderStateChanged notification);

};

#endif // TCPMESSAGESENDERSMANAGER_H
