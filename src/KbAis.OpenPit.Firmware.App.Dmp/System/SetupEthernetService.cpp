#include "SetupEthernetService.h"

// qt
#include <QFile>
#include <QSettings>

#include "JsonQt.h"
#include "Format.h"

using namespace std::chrono_literals;

const QString CONNMAN_SETTINGS_PATH { QStringLiteral("/media/app/connman/tiptruck.config") };

SetupEthernetService::SetupEthernetService(
    IRxConfigurationChangePublisher& configurationPublisher
) {
    configurationPublisher.getChangeObservable("ethernet")
        .sample_with_time(500ms, rxcpp::observe_on_event_loop())
        .subscribe(m_subscriptions, [this](const AppConfiguration& x) { handle(x); });
}

SetupEthernetService::~SetupEthernetService() {
    m_subscriptions.unsubscribe();
}

void
SetupEthernetService::handle(const AppConfiguration& new_config) {
    // QUESTION: how to configure connamn when building yocto
    const auto& j = new_config.j_object;

    const auto dns = j.at("dns").get<std::string>();
    const auto ip = j.at("ip").get<std::string>();
    const auto mask = j.at("mask").get<std::string>();
    const auto gateway = j.at("gateway").get<std::string>();
    const auto is_manual_enabled = j.at("manual_enable").get<bool>();

    QMutexLocker lock { &m_mtx_update_connman_profile };

    QSettings settings { CONNMAN_SETTINGS_PATH, QSettings::IniFormat };

    // Inserted is 'as is' from original TK-implementation (see ethernet_config.cpp).
    settings.clear();

    settings.beginGroup("global");
    settings.setValue("Name", "openpit");
    settings.endGroup();

    settings.beginGroup("service_openpit");
    settings.setValue("Type", "ethernet");
    settings.setValue("IPv4", is_manual_enabled ? QString::fromStdString(fmt::format("{}/{}/{}", ip, mask, gateway)) : "dhcp");
    settings.setValue("Nameservers", is_manual_enabled ? QString::fromStdString(dns) : "" );
    settings.endGroup();

    // TODO: Handle error
    settings.sync();
}
