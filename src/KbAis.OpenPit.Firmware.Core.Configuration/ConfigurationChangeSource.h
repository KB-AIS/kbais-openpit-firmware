#ifndef CONFIGURATIONCHANGESOURCE_H
#define CONFIGURATIONCHANGESOURCE_H

// qt
#include <QDir>
#include <QFileSystemWatcher>
#include <QObject>
#include <QReadWriteLock>
// oss
#include <nlohmann/json.hpp>
#include <rxcpp/rx.hpp>

#include "Configuration.h"

class ConfigurationManager;

class ConfigurationChangeSource {
    friend ConfigurationManager;

public:
    ConfigurationChangeSource(
        AppConfiguration configuration
    ,   const QString& configurationFilePath
    );

    ~ConfigurationChangeSource() noexcept;

    QString getConfigurationName() const;

    AppConfiguration getConfiguration();

    rxcpp::observable<AppConfiguration> getChangeObservable();

private:
    AppConfiguration mConfiguration;

    const QString mConfigurationFilePath;

    QFileSystemWatcher mChangeWatcher;

    rxcpp::composite_subscription mSubs;

    rxcpp::rxsub::behavior<AppConfiguration> mChangeSubject;

    QReadWriteLock mRwlConfigurationValue;

    QReadWriteLock mRwlConfigurationFile;

    void loadConfigurationValue();

    void setConfigurationValue(const nlohmann::json& newConfigurationValue);

};
#endif // CONFIGURATIONCHANGESOURCE_H
