#ifndef CONFIGURATIONSERVICE_H
#define CONFIGURATIONSERVICE_H

// qt
#include <QDir>
#include <QFileSystemWatcher>
#include <QMap>
#include <QObject>
#include <QReadWriteLock>
#include <QSharedPointer>
#include <QVector>
// oss
#include <nlohmann/json.hpp>

#include <RxQt/RxQt.h>

struct Configuration {

    QString name;

    nlohmann::json value;

};

class IRxConfigurationChangePublisher : virtual public QObject {

public:
    virtual ~IRxConfigurationChangePublisher() noexcept = default;

    virtual rxcpp::observable<Configuration>
    get_observable(const QString& configuration_name) const = 0;

};

class IConfigurationProvider : virtual public QObject {

public:
    virtual ~IConfigurationProvider() noexcept = default;

    virtual Configuration
    get_configuration(const QString& configuration_name) = 0;

};

class ConfigurationService
    :   public IRxConfigurationChangePublisher
    ,   public IConfigurationProvider {
    Q_OBJECT

public:
    ConfigurationService();

    void register_configuration(
        const QString& configuration_name
    ,   const nlohmann::json& default_value
    );

    void update_configuration(const QString& configuration_name, const nlohmann::json& value);

    rxcpp::observable<Configuration> get_observable(
        const QString& configuration_name
    ) const override;

    Configuration get_configuration(const QString& configuration_name) override;

private:
    const QDir m_configuration_location;

    rxcpp::composite_subscription m_subs;

    std::map<QString, std::shared_ptr<QFileSystemWatcher>> m_configuration_file_watcher;

    std::map<QString, std::shared_ptr<rxcpp::rxsub::behavior<Configuration>>> m_subjects;

    QReadWriteLock lck_configuration_file;

    QFileInfo get_configuration_file_info(const QString& configuration_name);

    void write_configuration_value(const QString& configuration_name, const nlohmann::json& json);

    nlohmann::json get_configuration_value(const QString& configuration_name);

};

#endif // CONFIGURATIONSERVICE_H
