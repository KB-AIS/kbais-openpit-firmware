#ifndef APP_CONFIGURATION_H
#define APP_CONFIGURATION_H

// qt
#include <QString>
// oss
#include <nlohmann/json.hpp>

struct app_configuration {

    /*!
     * \brief Наименование файла конфигурации приложения.
     */
    QString name;

    /*!
     * \brief Значение конфигурации в формате JSON.
     */
    nlohmann::json value;

};

#endif // APP_CONFIGURATION_H
