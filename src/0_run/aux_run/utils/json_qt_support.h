#ifndef JSON_QT_SUPPORT_H
#define JSON_QT_SUPPORT_H

// qt
#include <QDateTime>
#include <QString>
// oss
#include <nlohmann/json.hpp>

namespace nlohmann {

template<>
struct adl_serializer<QString> {
    static void to_json(json& j, const QString& string) {
        j = string.toStdString();
    }

    static void from_json(const json& j, QString& string) {
        if (!j.is_string()) {
            return;
        }

        string = j;
    }
};

template<>
struct adl_serializer<QDateTime> {
    static void to_json(json& j, const QDateTime& datetime) {
        j = datetime.toString();
    }

    static void from_json(const json& j, QDateTime& datetime) {
        if (!j.is_string()) {
            return;
        }

        datetime = QDateTime::fromString(j);
    }
};

}

#endif // JSON_QT_SUPPORT_H
