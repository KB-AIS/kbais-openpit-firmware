#include "ConfigurationService.h"

#include <QFileSystemWatcher>
#include <plog/Log.h>

ConfigurationService::ConfigurationService()
    :   QObject()
    ,   IRxConfigurationChangePublisher()
    ,   IConfigurationProvider()
    ,   m_configuration_location { "/media/app/cfw/configuration" }
{
    PLOGD << "ctor";
    Q_ASSERT(m_configuration_location.isAbsolute());
    Q_ASSERT(m_configuration_location.exists());
}

void
ConfigurationService::register_configuration(
    const QString& configuration_name
,   const nlohmann::json& default_value
) {
    const auto configuration_file_info = get_configuration_file_info(configuration_name);

    nlohmann::json confiugration_value;

    // Create configuration file with default value if it does not exist
    if (!configuration_file_info.exists()) {
        write_configuration_value(configuration_name, default_value);
        confiugration_value = default_value;
    } else {
        confiugration_value = get_configuration_value(configuration_name);
    }

    Configuration configuration { configuration_name, confiugration_value };

    auto ptr_configuration_file_watcher = std::make_shared<QFileSystemWatcher>(this);

    ptr_configuration_file_watcher->addPath(configuration_file_info.filePath());

    m_configuration_file_watcher[configuration_name] = ptr_configuration_file_watcher;

    auto sub_configuration_change = rxcpp::rxsub::behavior<Configuration>(configuration);

    m_subjects[configuration_name] =
        std::make_shared<rxcpp::rxsub::behavior<Configuration>>(sub_configuration_change);

    rxqt::from_signal<1>(ptr_configuration_file_watcher.get(), &QFileSystemWatcher::fileChanged)
        .subscribe(
            m_subs
        ,   [&](QString configuration_file_name) {
                QFileInfo configuration_file_info { configuration_file_name };

                const auto configuration_name = configuration_file_info.baseName();

                const auto subsciber = m_subjects[configuration_name]->get_subscriber();

                const auto configuration_value = get_configuration_value(configuration_name);

                subsciber.on_next(Configuration { configuration_name, configuration_value });
            }
        );
}

void
ConfigurationService::update_configuration(
    const QString& configuration_name
,   const nlohmann::json& value
) {
    write_configuration_value(configuration_name, value);
}

rxcpp::observable<Configuration>
ConfigurationService::get_observable(
    const QString& configuration_name
) const {
    const auto itr_find_configuration_change = m_subjects.find(configuration_name);

    if (itr_find_configuration_change == m_subjects.end()) {
        throw std::range_error(configuration_name.toStdString());
    }

    return itr_find_configuration_change->second->get_observable();
}

Configuration
ConfigurationService::get_configuration(const QString& configuration_name) {
    return {
        configuration_name,
        get_configuration_value(configuration_name)
    };
}

QFileInfo
ConfigurationService::get_configuration_file_info(const QString& configuration_name) {
    const auto configuration_file_name = QString("%1.json").arg(configuration_name);

    const auto configuration_file_info =
        QFileInfo { m_configuration_location.filePath(configuration_file_name) };

    return configuration_file_info;
}

void
ConfigurationService::write_configuration_value(
    const QString& configuration_name
,   const nlohmann::json& json
) {
    const auto configuration_file_info = get_configuration_file_info(configuration_name);

    QWriteLocker lock(&lck_configuration_file);

    QFile configuration_file { configuration_file_info.filePath() };

    configuration_file.open(QIODevice::WriteOnly);
    configuration_file.write(json.dump(4).c_str());
    configuration_file.close();
}

nlohmann::json
ConfigurationService::get_configuration_value(const QString& configuration_name) {
    const auto configuration_file_info = get_configuration_file_info(configuration_name);

    QReadLocker lock(&lck_configuration_file);

    QFile configuration_file { configuration_file_info.filePath() };

    configuration_file.open(QIODevice::ReadOnly);
    auto value = nlohmann::json::parse(configuration_file.readAll().toStdString());
    configuration_file.close();

    return value;
}
