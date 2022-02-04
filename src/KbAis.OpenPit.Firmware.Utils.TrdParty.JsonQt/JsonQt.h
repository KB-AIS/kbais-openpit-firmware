#ifndef JSONQT_H
#define JSONQT_H

#include <nlohmann/json.hpp>

#include <QtCore/qdatetime.h>
#include <QtCore/qglobal.h>
#include <QtCore/qlist.h>
#include <QtCore/qstring.h>
#include <QtCore/qurl.h>
#include <QtCore/qvariant.h>
#include <QtCore/qvector.h>

#include <string>

QT_BEGIN_NAMESPACE

inline void from_json(const nlohmann::json &j, QString &string) {
    string = QString::fromStdString(j.get<std::string>());
}

inline void from_json(const nlohmann::json &j, QDateTime &dateTime) {
    dateTime = QDateTime::fromString(j.get<QString>(), Qt::ISODateWithMs);
}

inline void from_json(const nlohmann::json &j, QUrl &url) {
    url.setUrl(j.get<QString>());
}

inline void from_json(const nlohmann::json &j, QVariant& variant) {
    if (j.is_null()) {
        variant = QVariant::fromValue(nullptr);
    } else if (j.is_number_unsigned()) {
        variant = QVariant::fromValue(j.get<quint64>());
    } else if (j.is_number_integer()) {
        variant = QVariant::fromValue(j.get<qint64>());
    } else if (j.is_number_float()) {
        variant = QVariant::fromValue(j.get<qreal>());
    } else if (j.is_string()) {
        variant = QVariant::fromValue(j.get<QString>());
    } else if (j.is_boolean()) {
        variant = QVariant::fromValue(j.get<bool>());
    }
}

template<typename T>
inline void from_json(const nlohmann::json &j, QVector<T> &vector) {
    for (const auto &item : j) {
        vector.push_back(item.get<T>());
    }
}

template<typename T>
inline void from_json(const nlohmann::json &j, QList<T> &list) {
    for (const auto &item : j) {
        list.push_back(item.get<T>());
    }
}

inline void to_json(nlohmann::json& j, const QVariant&);

inline void to_json(nlohmann::json& j, const QString& string) {
    j = string.toStdString();
}

inline void to_json(nlohmann::json& j, const QVariantList& list) {
    j = nlohmann::json::array();

    for (auto it = std::cbegin(list); it != std::cend(list); ++it) {
        nlohmann::json serialized;
        to_json(serialized, *it);
        j.push_back(serialized);
    }
}

inline void to_json(nlohmann::json& j, const QVariantMap& map) {
    j = nlohmann::json::object();

    for (auto it = std::cbegin(map); it != std::cend(map); ++it) {
        nlohmann::json serialized;
        to_json(serialized, it.value());
        j[it.key().toStdString()] = serialized;
    }
}

inline void to_json(nlohmann::json& j, const QVariant& variant) {
    auto type = static_cast<QMetaType::Type>(variant.type());

    if (type == QMetaType::QVariantList) {
        to_json(j, variant.value<QVariantList>());
    } else if (type == QMetaType::QVariantMap) {
        to_json(j, variant.value<QVariantMap>());
    } else if (type == QMetaType::QDateTime) {
        j = variant.toDateTime().toString(Qt::ISODateWithMs).toStdString();
    } else if (type == QMetaType::QString) {
        j = variant.toString().toStdString();
    } else if (type == QMetaType::Double) {
        j = variant.toDouble();
    } else if (type == QMetaType::Bool) {
        j = variant.toBool();
    } else if (type == QMetaType::Int) {
        j = variant.toInt();
    } else if (type == QMetaType::UInt) {
        j = variant.toUInt();
    } else {
        j.clear();
    }
}

QT_END_NAMESPACE

#endif // JSONQT_H
