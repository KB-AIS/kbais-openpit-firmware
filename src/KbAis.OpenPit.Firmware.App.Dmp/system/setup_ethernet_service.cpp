#include "setup_ethernet_service.h"

// qt
#include <QSettings>
// oss
#include <plog/Log.h>

#include "JsonQt.h"
#include "Format.h"

using namespace std::chrono_literals;

// QUESTION: how to configure connamn when building yocto
const QString CONNMAN_SETTINGS_PATH { QStringLiteral("/media/app/connman/tiptruck.config") };

setup_ethernet_service::setup_ethernet_service(
    const i_app_configuration_publisher& app_configuration_publisher
) {
    app_configuration_publisher.get_observable(QStringLiteral("ethernet"))
        .sample_with_time(500ms, rxcpp::observe_on_event_loop()) // TODO: Run in main thread
        .map(&setup_ethernet_service::map_ethernet_configuration)
        .filter([](const auto& x) -> bool {
            return x.has_value();
        })
        .tap([](const auto&) {
            PLOGV << "Got a new ethernet configuration to apply";
        })
        .subscribe(subscriptions_, &setup_ethernet_service::handle_new_ethnet_config);
}

setup_ethernet_service::~setup_ethernet_service() {
    subscriptions_.unsubscribe();
}

setup_ethernet_service::ethnet_config_opt_t
setup_ethernet_service::map_ethernet_configuration(const app_configuration& c) {
    try {
        const auto& c_v = c.value;

        return std::make_optional(ethnet_config {
            .dns               = c_v.at("dns").get<std::string>()
        ,   .ip                = c_v.at("ip").get<std::string>()
        ,   .mask              = c_v.at("mask").get<std::string>()
        ,   .gateway           = c_v.at("gateway").get<std::string>()
        ,   .is_manual_enabled = c_v.at("manual_enable").get<bool>()
        });
    } catch(std::exception& exception) {
        PLOGE << "An exception occured during ethernet configuration mapping: " <<  exception.what();
    }

    return std::nullopt;
}

void
setup_ethernet_service::handle_new_ethnet_config(const ethnet_config_opt_t& conf_opt) {
    constexpr auto form_ipv4_qstr = [](const ethnet_config& x) -> QString {
        return x.is_manual_enabled
            ?   QString::fromStdString(fmt::format("{}/{}/{}", x.ip, x.mask, x.gateway))
            :   QStringLiteral("dhcp");
    };

    constexpr auto form_nameservers_qstr = [](const ethnet_config& x) -> QString {
        return x.is_manual_enabled ? QString::fromStdString(x.dns) : QString();
    };

    const auto& conf = conf_opt.value();

    QSettings settings { CONNMAN_SETTINGS_PATH, QSettings::IniFormat };

    settings.clear();

    // Для референса: https://manpages.debian.org/unstable/connman/connman-service.config.5.en.html
    // Запись 'global' секции конфигурации connman
    settings.beginGroup(QStringLiteral("global"));
    settings.setValue(QStringLiteral("Name"), "opf");
    settings.endGroup();
    // Запись 'ethernet' секции настроек подключения сети
    settings.beginGroup(QStringLiteral("service_opf_ethernet"));
    settings.setValue(QStringLiteral("Type"), QStringLiteral("ethernet"));
    settings.setValue(QStringLiteral("IPv4"), form_ipv4_qstr(conf));
    settings.setValue(QStringLiteral("Nameservers"), form_nameservers_qstr(conf));
    settings.endGroup();

    settings.sync();

    const auto settings_status = settings.status();
    if (settings_status == QSettings::NoError) {
        PLOGI << "New ethernet configuration has been synchronized";
        return;
    }

    PLOGW << "A error has been produced at new ethernet configuration synchronization";
}
