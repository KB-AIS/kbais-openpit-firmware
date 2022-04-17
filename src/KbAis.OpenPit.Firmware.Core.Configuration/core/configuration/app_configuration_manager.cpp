#include "app_configuration_manager.h"

// oss
#include <plog/Log.h>
#include <range/v3/all.hpp>

const QString CONFIGURATION_DIRECORY { QStringLiteral("/media/app/cfw/configuration") };

app_configuration_manager::app_configuration_manager()
    :   configuration_directory { CONFIGURATION_DIRECORY }
{
}

void
app_configuration_manager::register_by_name(const QString& name, const nlohmann::json& default_value) {
    const auto is_registered = ranges::contains(configuration_watchers | ranges::views::keys, name);

    if (is_registered) throw std::logic_error("name");

    const auto configuration_file_name = QString(QStringLiteral("%1.json")).arg(name);
    const QFileInfo file_info { configuration_directory.filePath(configuration_file_name) };

    nlohmann::json configuraiton_value;

    if (!file_info.exists()) {
        QFile configuration_file { file_info.filePath() };

        configuration_file.open(QIODevice::WriteOnly);
        configuration_file.write(default_value.dump(4).c_str());

        configuration_file.close();

        configuraiton_value = default_value;
    } else {
        configuraiton_value = load_configuration_value(file_info);
    }

    configuration_watchers[name].reset(new app_configuration_watcher(
        app_configuration { name, configuraiton_value }
    ,   file_info.filePath()
    ));
}

void
app_configuration_manager::set_configuration(const QString& name, const nlohmann::json& value) {
    const auto v = configuration_watchers | ranges::views::keys;

    const auto configuration_key = ranges::find(v, name);
    if (configuration_key == ranges::end(v)) throw std::out_of_range("name");

    return configuration_watchers.at(*configuration_key)->set_configuration_value(value);
}

app_configuration
app_configuration_manager::get_configuration(const QString& name) const {
    const auto v = configuration_watchers | ranges::views::keys;

    const auto configuration_key = ranges::find(v, name);
    if (configuration_key == ranges::end(v)) throw std::out_of_range("name");

    return configuration_watchers.at(*configuration_key)->get_configuraiton();
}

rxcpp::observable<app_configuration>
app_configuration_manager::get_observable(const QString& name) const {
    const auto v = configuration_watchers | ranges::views::keys;

    const auto configuration_key = ranges::find(v, name);
    if (configuration_key == ranges::end(v)) throw std::out_of_range("name");

    return configuration_watchers.at(*configuration_key)->get_observable();
}

nlohmann::json
app_configuration_manager::load_configuration_value(const QFileInfo& file_info) {
    QFile configuration_file { file_info.filePath() };

    configuration_file.open(QIODevice::ReadOnly);
    auto configuration_value = configuration_file.readAll();

    configuration_file.close();

    if (configuration_value.isNull()) return { };

    return nlohmann::json::parse(configuration_value.toStdString());
}
