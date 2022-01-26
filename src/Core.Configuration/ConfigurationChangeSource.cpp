#include "ConfigurationChangeSource.h"

// oss
#include <plog/Log.h>

#include "RxQt/RxQt.h"

ConfigurationChangeSource::ConfigurationChangeSource(
    Configuration configuration
,   const QString& configurationFilePath
)
    :   mConfiguration { configuration }
    ,   mConfigurationFilePath { configurationFilePath }
    ,   mChangeWatcher { QStringList { configurationFilePath } }
    ,   mChangeSubject(configuration)
{
    mSubs = rxcpp::composite_subscription();
    rxqt::from_signal<1>(&mChangeWatcher, &QFileSystemWatcher::fileChanged)
        .subscribe(
            mSubs
        ,   [&](auto) {
                loadConfigurationValue();
                const auto subscriber = mChangeSubject.get_subscriber();
                subscriber.on_next(getConfiguration());
            }
        );
}

ConfigurationChangeSource::~ConfigurationChangeSource() noexcept {
    if (!mSubs.is_subscribed()) return;

    mSubs.unsubscribe();
}

QString
ConfigurationChangeSource::getConfigurationName() const {
    return mConfiguration.name;
}

Configuration
ConfigurationChangeSource::getConfiguration() {
    QReadLocker lock(&mRwlConfigurationValue);

    return mConfiguration;
}

rxcpp::observable<Configuration>
ConfigurationChangeSource::getChangeObservable() {
    return mChangeSubject.get_observable();
}

void
ConfigurationChangeSource::loadConfigurationValue() {
    QFile configurationFile { mConfigurationFilePath };

    QByteArray configurationFileValue;

    mRwlConfigurationFile.lockForRead();
    try {
        configurationFile.open(QIODevice::ReadOnly);
        configurationFileValue = configurationFile.readAll();
    }  catch (...) { }
    configurationFile.close();
    mRwlConfigurationFile.unlock();

    if (configurationFileValue.isEmpty()) return;

    QWriteLocker lock(&mRwlConfigurationValue);
    mConfiguration.value = nlohmann::json::parse(configurationFileValue.toStdString());
}

void
ConfigurationChangeSource::setConfigurationValue(const nlohmann::json& newConfigurationValue) {
    QFile configurationFile { mConfigurationFilePath };

    QWriteLocker lock(&mRwlConfigurationFile);
    try {
        configurationFile.open(QIODevice::WriteOnly);
        configurationFile.write(newConfigurationValue.dump(4).c_str());
    }  catch (...) { }
    configurationFile.close();
}
