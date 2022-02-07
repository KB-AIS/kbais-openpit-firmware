#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// qt
#include <QString>
// oss
#include <nlohmann/json.hpp>

struct AppConfiguration {
    QString section_name;

    nlohmann::json j_object;

    AppConfiguration(QString name, nlohmann::json value)
        :   section_name(name)
        ,   j_object(value)
    {

    }

};

#endif // CONFIGURATION_H
