#ifndef APP_CONFIGURATION_MANAGER_H
#define APP_CONFIGURATION_MANAGER_H

// qt
#include <QDir>
#include <QObject>

#include "app_configuration_watcher.h"

/*!
 * \brief Интерефейс для доступа к значению конфигурации приложения по имени.
 */
class i_app_configuration_provider {

public:
    virtual ~i_app_configuration_provider() noexcept = default;

    virtual app_configuration get_configuration(const QString& name) const = 0;

};

/*!
 * \brief Интефрейс для доступа к публикациям изменений в конфигурации 
 * приложения по имени.
 */
class i_app_configuration_publisher {

public:
    virtual ~i_app_configuration_publisher() noexcept = default;

    virtual rxcpp::observable<app_configuration> get_observable(const QString& name) const = 0;

};

class app_configuration_manager
    :   public i_app_configuration_provider
    ,   public i_app_configuration_publisher
{
    using configuration_watchers_t = std::map<QString, std::unique_ptr<app_configuration_watcher>>;

    const QDir configuration_directory;

    configuration_watchers_t configuration_watchers;

    QFileInfo get_configuration_file_info(const QString& name) const;

    nlohmann::json load_configuration_value(const QFileInfo& file_info);

public:
    app_configuration_manager();

    void register_by_name(const QString& name, const nlohmann::json& default_value);

    void              set_configuration(const QString& name, const nlohmann::json& value);

    app_configuration get_configuration(const QString& name) const override;

    rxcpp::observable<app_configuration> get_observable(const QString& name) const override;

};

#endif // APP_CONFIGURATION_MANAGER_H
