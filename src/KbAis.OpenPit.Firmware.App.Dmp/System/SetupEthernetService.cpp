#include "SetupEthernetService.h"

// qt
#include <QFile>
#include <QSettings>

#include "JsonQt.h"

const QString ETHERNET_CONNMAN_SETTINGS_PATH {
    QStringLiteral("/media/app/connman/tiptruck.config")
};

SetupEthernetService::SetupEthernetService(
    IRxConfigurationChangePublisher& configurationPublisher
) {
    // TODO: Use own thread run loop for system services
    configurationPublisher.getChangeObservable("ethernet")
        .subscribe(m_subsBag, [this](const AppConfiguration& x) { handle(x); });
}

SetupEthernetService::~SetupEthernetService() {
    m_subsBag.unsubscribe();
}

void SetupEthernetService::handle(const AppConfiguration& newConfiguration) {
    // !Q: how to configure connamn when building yocto
    const auto& j = newConfiguration.j_object;

    const auto dns = j.at("dns").get<QString>();
    const auto ip = j.at("ip").get<QString>();
    const auto mask = j.at("mask").get<QString>();
    const auto gateway = j.at("gateway").get<QString>();
    const auto isManualEnabled = j.at("manual_enable").get<bool>();

    const auto removed = QFile::remove(ETHERNET_CONNMAN_SETTINGS_PATH);

    QSettings settings { ETHERNET_CONNMAN_SETTINGS_PATH, QSettings::IniFormat };

    // Inserted is 'as is ' from original implementation (see ethernet_config.cpp).
    // Probably, need to do refactoring later.
    settings.clear();
    settings.beginGroup("global");
    settings.setValue("Name", "tiptruck");
    settings.endGroup();

    settings.beginGroup("service_tiptruck");
    settings.setValue("Type", "ethernet");
    QString setting_ip;

    if (!isManualEnabled) {
       setting_ip = "dhcp";
    } else {
       setting_ip = ip + "/" + mask + "/" + gateway;
       settings.setValue ("Nameservers", dns);
    }

    settings.setValue("IPv4", setting_ip);
    settings.endGroup();

    settings.sync();
}
