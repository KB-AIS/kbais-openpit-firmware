#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// qt
#include <QString>
// oss
#include <nlohmann/json.hpp>

struct Configuration {
    QString name;

    nlohmann::json value;

    Configuration(QString name, nlohmann::json value)
        :   name(name)
        ,   value(value)
    {

    }

};

#endif // CONFIGURATION_H
