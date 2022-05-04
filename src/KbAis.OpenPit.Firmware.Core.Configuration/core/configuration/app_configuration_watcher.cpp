#include "app_configuration_watcher.h"

// std
#include <chrono>
// qt
#include <QFile>
// oss
#include <plog/Log.h>

#include "rxqt.h"

using namespace std::chrono_literals;

app_configuration_watcher::app_configuration_watcher(
    app_configuration configuration
,   const QString& configuration_file_path
)
    :   configuration_file_path_ { configuration_file_path }
    ,   configuration_name_      { configuration.name }
    ,   configuration_subject_   { configuration }
{
    file_watcher_.addPath(configuration_file_path);

    rxqt::from_signal<0>(&file_watcher_, &QFileSystemWatcher::fileChanged)
        .subscribe(subscriptions_, [this](auto) { publish(); });
}

app_configuration_watcher::~app_configuration_watcher() noexcept {
    if (!subscriptions_.is_subscribed()) return;

    subscriptions_.unsubscribe();
}

QString
app_configuration_watcher::get_configuration_name() const {
    return configuration_name_;
}

app_configuration
app_configuration_watcher::get_configuraiton() {
    QReadLocker lock(&lock_configuration_value_);

    return configuration_subject_.get_value();
}

rxcpp::observable<app_configuration>
app_configuration_watcher::get_observable() const {
    return configuration_subject_.get_observable();
}

void
app_configuration_watcher::publish() {
    QFile configuration_file { configuration_file_path_ };

    QByteArray configuration_file_value;

    lock_configuration_file_.lockForRead();

    configuration_file.open(QIODevice::ReadOnly);
    configuration_file_value = configuration_file.readAll();
    configuration_file.close();

    lock_configuration_file_.unlock();

    auto configuration_value = nlohmann::json::parse(configuration_file_value.data());
    configuration_subject_.get_subscriber().on_next(app_configuration {
        .name  = configuration_name_
    ,   .value = std::move(configuration_value)
    });
}

void
app_configuration_watcher::set_configuration_value(const nlohmann::json& new_configuration_value) {
    QFile configuration_file { configuration_file_path_ };

    QWriteLocker lock(&lock_configuration_file_);

    configuration_file.open(QIODevice::WriteOnly);
    configuration_file.write(new_configuration_value.dump(4).c_str());
    configuration_file.close();
}
