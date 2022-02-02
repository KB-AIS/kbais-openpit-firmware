#include "ConfigurationsManager.h"

#include <QFileSystemWatcher>
#include <plog/Log.h>

ConfigurationManager::ConfigurationManager()
    :   QObject()
    ,   IConfigurationProvider()
    ,   IRxConfigurationChangePublisher()
    ,   mConfigurationLocation { "/media/app/cfw/configuration" }
{

}

void
ConfigurationManager::registerConfiguration(
    const QString& configurationName
,   const nlohmann::json& configurationDefaultValue
) {
    auto isConfigurationRegistered =
        mConfigurationChangeSources.find(configurationName) != mConfigurationChangeSources.end();
    if (isConfigurationRegistered) return;

    const auto configurationFileInfo = getConfigurationFileInfo(configurationName);

    nlohmann::json configurationValue;
    if (!configurationFileInfo.exists()) {
        createConfigurationFile(configurationFileInfo, configurationDefaultValue);
        configurationValue = configurationDefaultValue;
    } else {
        configurationValue = loadConfigurationValue(configurationFileInfo);
    }

    mConfigurationChangeSources[configurationName].reset(
        new ConfigurationChangeSource(
            AppConfiguration { configurationName, configurationValue }
        , configurationFileInfo.filePath()
        )
    );
}

void
ConfigurationManager::updateConfiguration(const QString& name, const nlohmann::json& value) {
    mConfigurationChangeSources[name]->setConfigurationValue(value);
}

AppConfiguration
ConfigurationManager::getConfiguration(const QString& name) {
    return mConfigurationChangeSources[name]->getConfiguration();
}

rxcpp::observable<AppConfiguration>
ConfigurationManager::getChangeObservable(const QString& name) {
    return mConfigurationChangeSources[name]->getChangeObservable();
}

QFileInfo
ConfigurationManager::getConfigurationFileInfo(const QString& configurationName) const {
    const auto configurationFileName = QString("%1.json").arg(configurationName);

    const auto configurationFileInfo =
        QFileInfo { mConfigurationLocation.filePath(configurationFileName) };

    return configurationFileInfo;
}

nlohmann::json
ConfigurationManager::loadConfigurationValue(const QFileInfo& configurationFileInfo) {
    QFile configurationFile { configurationFileInfo.filePath() };

    QByteArray configurationFileValue;

    try {
        configurationFile.open(QIODevice::ReadOnly);
        configurationFileValue = configurationFile.readAll();
    }  catch (...) { }
    configurationFile.close();

    if (configurationFileValue.isNull()) return { };

    return nlohmann::json::parse(configurationFileValue.toStdString());
}

void
ConfigurationManager::createConfigurationFile(
    const QFileInfo& configurationFileInfo
,   const nlohmann::json& confiugrationDefaultValue
) {
    QFile configurationFile { configurationFileInfo.filePath() };

    try {
        configurationFile.open(QIODevice::WriteOnly);
        configurationFile.write(confiugrationDefaultValue.dump(4).c_str());
    }  catch (...) { }
    configurationFile.close();
}
