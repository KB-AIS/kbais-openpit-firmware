#include "TcpMessageSendersManager.h"

// qt
#include "QMetaEnum"
// oss
#include <fmt/format.h>
#include <plog/Log.h>
#include <range/v3/all.hpp>

#include "Format.h"
#include "JsonQt.h"

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds RESTART_MESSAGE_SENDERS_PERIOD { 5s };

template<typename QEnum>
QString QtEnumToString(const QEnum value) {
    return QMetaEnum::fromType<QEnum>().valueToKey(value);
}

TcpMessageSendersManager::TcpMessageSendersManager(
    const i_app_configuration_publisher& app_configuration_publisher
)
    :   app_configuration_publisher_(app_configuration_publisher)
    ,   m_subMessageSenderDiagInfosChanged({ })
{
    m_subsConfigurationChanged = rxcpp::composite_subscription();
}

void
TcpMessageSendersManager::StartWorkOn(const rxcpp::observe_on_one_worker& coordination) {
    PLOGD << "starting work";

    app_configuration_publisher_.get_observable("base")
        // TODO: Use aggregate function to make a strong-typed config
        .combine_latest(app_configuration_publisher_.get_observable("networking"))
        .sample_with_time(500ms, coordination)
        .subscribe(
            m_subsConfigurationChanged
        ,   [&](const std::tuple<app_configuration, app_configuration> x) {
                constexpr int CONFIG_BASE = 0, CONFIG_NETW = 1;

                m_equipment_id = std::get<CONFIG_BASE>(x).value
                    .at("/vehihle_id"_json_pointer).get<QString>();

                OnConfigurationChanged(std::get<CONFIG_NETW>(x));
            }
        );

    m_obsMessageSednersRestartInterval =
        rxcpp::observable<>::interval(RESTART_MESSAGE_SENDERS_PERIOD, coordination);
}

rxcpp::observable<std::vector<MessageSenderDiagInfo>>
TcpMessageSendersManager::GetObservableDiagInfo() const {
    return m_subMessageSenderDiagInfosChanged.get_observable();
}

void
TcpMessageSendersManager::OnConfigurationChanged(const app_configuration& configuration) {
    PLOGV << "got a new 'networking' configuration";

    // Stop getting old notifications
    m_subsMessageSenderStatesChanged.unsubscribe();

    m_subsMessageSenderStatesChanged = rxcpp::composite_subscription();

    PLOGV << "stopping message senders...";

    m_messageSenders.clear();
    m_messageSenderStates.clear();

    m_messageSenderConfigurations = configuration.value.at("/servers"_json_pointer)
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

    PLOGV << fmt::format("got {} sender configuration(s)", m_messageSenderConfigurations.size());

    ranges::for_each(
        m_messageSenderConfigurations | ranges::views::values
    ,   [&](const TcpMessageSenderConfiguration& x) {
            const auto id = x.GetMessageSenderName();

            PLOGV << fmt::format("{} message sender is being created", id.toStdString());

            // TODO: create message sender form factory
            m_messageSenders[id].reset(new TcpMessageSender { id });

            // Set default state - unconnected
            m_messageSenderStates[id] = { };
        }
    );

    rxcpp::observable<TcpMessageSenderStateChanged> obsMessageSenderStatesChanged =
        rxcpp::observable<>::empty<TcpMessageSenderStateChanged>();

    ranges::for_each(
        m_messageSenders | ranges::views::values
    ,   [&o = obsMessageSenderStatesChanged](const auto& x) {
            o = rxqt::from_signal(x.get(), &TcpMessageSender::StateChanged).merge(o);
        }
    );

    obsMessageSenderStatesChanged.subscribe(
        m_subsMessageSenderStatesChanged
    ,   [&](const TcpMessageSenderStateChanged& x) { OnMessageSenderStateChanged(x); }
    );


    m_obsMessageSednersRestartInterval.subscribe(
        m_subsMessageSenderStatesChanged
    ,   [&](auto) { OnMessageSendersRestartRequired(); }
    );
}

void
TcpMessageSendersManager::OnMessageSendersRestartRequired() {
    ranges::for_each(
        m_messageSenderStates
    |   ranges::views::filter([&](const MessageSenderState_t& x) {
            return x.second.socket_state == QAbstractSocket::UnconnectedState;
        })
    |   ranges::views::keys
    ,   [&](const MessageSenderId_t& x) {
            PLOGV << fmt::format("{} message sender is being restared", x.toStdString());

            m_messageSenders[x]->Restart(m_messageSenderConfigurations[x], m_equipment_id);
        }
    );
}

void
TcpMessageSendersManager::OnMessageSenderStateChanged(
    TcpMessageSenderStateChanged notification
) {
    PLOGV_(1) << fmt::format(
        "{} message sender status changed to {}"
    ,   notification.messageSenderName.toStdString()
    ,   QtEnumToString(notification.state.socket_state).toStdString()
    );

    m_messageSenderStates[notification.messageSenderName] = notification.state;

    // Notify diagnositc info has been changed
    m_subMessageSenderDiagInfosChanged.get_subscriber().on_next(
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
