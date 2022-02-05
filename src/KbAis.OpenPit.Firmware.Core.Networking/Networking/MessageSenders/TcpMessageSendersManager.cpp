#include "TcpMessageSendersManager.h"

// qt
#include "QMetaEnum"
// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

// Utils.TrdParty.JsonQt
#include "JsonQt.h"
// Utils.TrdParty.RxQt
#include "RxQt.h"

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds RESTART_MESSAGE_SENDERS_PERIOD { 5s };

template<typename QEnum>
QString QtEnumToString(const QEnum value) {
    return QMetaEnum::fromType<QEnum>().valueToKey(value);
}

TcpMessageSendersManager::TcpMessageSendersManager(
    IRxConfigurationChangePublisher& configurationPublisher
)
    :   QObject()
    ,   m_configurationPublisher(configurationPublisher)
    ,   m_subject({ })
{

}

void
TcpMessageSendersManager::StartWorkOn(rxcpp::observe_on_one_worker& coordination) {
    m_subs = rxcpp::composite_subscription();

    auto const onConfigurationChanged_f =
        std::bind(&TcpMessageSendersManager::OnConfigurationChanged, this, std::placeholders::_1);
    m_configurationPublisher.getChangeObservable("networking")
        // Workaround: nasty way to prevent fireing on file change twice
        .sample_with_time(500ms, coordination)
        .subscribe(m_subs, onConfigurationChanged_f);

    auto const onMessageSendersRestartRequired_f =
        std::bind(&TcpMessageSendersManager::OnMessageSendersRestartRequired, this);
    rxcpp::observable<>::interval(RESTART_MESSAGE_SENDERS_PERIOD, coordination)
        .subscribe(m_subs, onMessageSendersRestartRequired_f);
}

rxcpp::observable<std::vector<MessageSenderDiagInfo>>
TcpMessageSendersManager::GetObservableDiagInfo() const {
    return m_subject.get_observable();
}

void
TcpMessageSendersManager::OnConfigurationChanged(
    AppConfiguration configuration
) {
    m_subsMessageSenderStateCahnge.unsubscribe();
    m_subsMessageSenderStateCahnge = rxcpp::composite_subscription();

    m_messageSenderStates.clear();

    m_messageSenderConfigurations.clear();

    // Stop message senders
    m_messageSenders.clear();

    m_messageSenderConfigurations = configuration.j_object.at("/servers"_json_pointer)
    |   ranges::views::transform([](nlohmann::json x) {
            TcpMessageSenderConfiguration msc {
                x.at("server_name").get<QString>()
            ,   x.at("server_port").get<quint16>()
            ,   MessageSenderProtocol::Swom
            ,   x.at("enabled").get<bool>()
            ,   std::chrono::seconds(x.at("send_interval").get<quint16>())
            };

            return std::make_pair(msc.GetMessageSenderName(), std::move(msc));
        })
    |   ranges::to<MessageSenderConfigurations_t>();

    const auto onMessageSenderStateChanged_f =
        std::bind(&TcpMessageSendersManager::OnMessageSenderStateChanged, this, std::placeholders::_1);

    ranges::for_each(
        m_messageSenderConfigurations
    |   ranges::views::values
    ,   [&](TcpMessageSenderConfiguration x) {
            const auto id = x.GetMessageSenderName();

            m_messageSenders[id].reset(new TcpMessageSender { id });
            // Set default state: unconnected
            m_messageSenderStates[id] = { };

            rxqt::from_signal(m_messageSenders[id].get(), &TcpMessageSender::StateChanged)
                .subscribe(m_subsMessageSenderStateCahnge, onMessageSenderStateChanged_f);
        }
    );
}

void
TcpMessageSendersManager::OnMessageSendersRestartRequired() {
    ranges::for_each(
        m_messageSenderStates
    |   ranges::views::filter([&](const MessageSenderState_t& x) {
            const auto has_message_sender_invalid_state =
                x.second.socket_state == QAbstractSocket::UnconnectedState;

            return has_message_sender_invalid_state;
        })
    |   ranges::views::keys
    ,   [&](const MessageSenderId_t& x) {
            m_messageSenders[x]->Restart(m_messageSenderConfigurations[x]);
        }
    );
}

void
TcpMessageSendersManager::OnMessageSenderStateChanged(
    TcpMessageSenderStateChanged notification
) {
    m_messageSenderStates[notification.message_sender_name] = notification.state;

    // Notify, diagnositc info changed
    m_subject.get_subscriber().on_next(
        m_messageSenderStates
    |   ranges::views::transform([&](const MessageSenderState_t& x) {
            return MessageSenderDiagInfo {
                x.first
            ,   QtEnumToString(x.second.socket_state)
            ,   x.second.socket_error != QAbstractSocket::UnknownSocketError
            ?   std::make_optional(QtEnumToString(x.second.socket_error))
            :   std::nullopt
            };
        })
    |   ranges::to<std::vector<MessageSenderDiagInfo>>()
    );
}
