#ifndef APP_CONFIGURATION_WATCHER_H
#define APP_CONFIGURATION_WATCHER_H

// qt
#include <QFileSystemWatcher>
#include <QReadWriteLock>
// oss
#include <rxcpp/rx.hpp>

#include "app_configuration.h"

class app_configuration_manager;

class app_configuration_watcher {
    friend app_configuration_manager;

    const QString configuration_file_path_;

    const QString configuration_name_;

    QFileSystemWatcher file_watcher_;

    rxcpp::composite_subscription subscriptions_;

    rxcpp::rxsub::behavior<app_configuration> configuration_subject_;

    QReadWriteLock lock_configuration_value_;

    QReadWriteLock lock_configuration_file_;

    void publish();

    void set_configuration_value(const nlohmann::json& new_configuration_value);

public:
    app_configuration_watcher(app_configuration configuration, const QString& configuration_file_path);

    ~app_configuration_watcher() noexcept;

    QString get_configuration_name() const;

    app_configuration get_configuraiton();

    rxcpp::observable<app_configuration> get_observable() const;

};
#endif // APP_CONFIGURATION_WATCHER_H
