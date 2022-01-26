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
        Configuration configuration
    ,   const QString& configurationFilePath
    );

    ~ConfigurationChangeSource() noexcept;

    QString getConfigurationName() const;

    Configuration getConfiguration();

    rxcpp::observable<Configuration> getChangeObservable();

private:
    Configuration mConfiguration;

    const QString mConfigurationFilePath;

    QFileSystemWatcher mChangeWatcher;

    rxcpp::composite_subscription mSubs;

    rxcpp::rxsub::behavior<Configuration> mChangeSubject;

    QReadWriteLock mRwlConfigurationValue;

    QReadWriteLock mRwlConfigurationFile;

    void loadConfigurationValue();

    void setConfigurationValue(const nlohmann::json& newConfigurationValue);

};
#endif // CONFIGURATIONCHANGESOURCE_H
