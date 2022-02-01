#ifndef UTILS_H
#define UTILS_H

// std
#include <optional>
// qt
#include <QRegularExpression>
// oss
#include <nlohmann/json.hpp>

static const QRegularExpression REQUEST_PARAM_CAPTURE { "([\\w]+):(.*)" };

constexpr int MANDATORY_PARAM_LENGTH = 2;

inline std::optional<nlohmann::json> extract_configuration(const QString& payload) {
    const auto match = REQUEST_PARAM_CAPTURE.match(payload);

    if (match.capturedLength() < MANDATORY_PARAM_LENGTH) {
        return std::nullopt;
    }

    const auto configuration = match.captured(2);

    if (configuration == "?") {
        return std::nullopt;
    }

    const auto rawConfiguration = configuration.toUtf8();

    return nlohmann::json::parse(rawConfiguration.constData());
}


#endif // UTILS_H
