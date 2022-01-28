#ifndef CONFIGURATIONSMANAGER_H
#define CONFIGURATIONSMANAGER_H

// qt
#include <QDir>
#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include <QVector>

#include "Configuration.h"
#include "ConfigurationChangeSource.h"
#include "IConfigurationProvider.h"
#include "IRxConfigurationChangePublisher.h"

class ConfigurationManager
    :   public QObject
    ,   public IConfigurationProvider
    ,   public IRxConfigurationChangePublisher {
    Q_OBJECT

public:
    ConfigurationManager();

    void registerConfiguration(const QString& name, const nlohmann::json& defaultValue);

    void updateConfiguration(const QString& name, const nlohmann::json& value);

    Configuration getConfiguration(const QString& name) override;

    rxcpp::observable<Configuration> getChangeObservable(const QString& name) override;

private:
    const QDir mConfigurationLocation;

    std::map<QString, std::unique_ptr<ConfigurationChangeSource>> mConfigurationChangeSources;

    QFileInfo getConfigurationFileInfo(const QString& configurationName) const;

    nlohmann::json loadConfigurationValue(const QFileInfo& configurationFileInfo);

    void createConfigurationFile(
        const QFileInfo& configurationFileInfo
    ,   const nlohmann::json& confiugrationDefaultValue
    );

};

#endif // CONFIGURATIONSMANAGER_H
