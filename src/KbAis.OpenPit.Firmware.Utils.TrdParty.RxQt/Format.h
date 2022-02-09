// TODO: Move to 'qt third party integration library'
#ifndef FORMAT_H
#define FORMAT_H

#define FMT_HEADER_ONLY

// std
#include <string_view>
// qt
#include <QString>
// oss
#include <fmt/format.h>

QT_BEGIN_NAMESPACE

inline std::string_view to_string_view(const QString& s) noexcept {
    return { s.toStdString() };
}

QT_END_NAMESPACE

#endif // FORMAT_H
