#include "ConfigurationChangeSource.h"

// oss
#include <plog/Log.h>

// Utils.TrdParty.RxQt
#include "RxQt.h"

ConfigurationChangeSource::ConfigurationChangeSource(
    AppConfiguration configuration
,   const QString& configurationFilePath
)
    :   mConfiguration { configuration }
    ,   mConfigurationFilePath { configurationFilePath }
    ,   mChangeSubject(configuration)
{
    mSubs = rxcpp::composite_subscription();

    mChangeWatcher.addPath(configurationFilePath);

    rxqt::from_signal<0>(&mChangeWatcher, &QFileSystemWatcher::fileChanged)
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
    return mConfiguration.section_name;
}

AppConfiguration
ConfigurationChangeSource::getConfiguration() {
    QReadLocker lock(&mRwlConfigurationValue);

    return mConfiguration;
}

rxcpp::observable<AppConfiguration>
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
    mConfiguration.j_object = nlohmann::json::parse(configurationFileValue.toStdString());
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
