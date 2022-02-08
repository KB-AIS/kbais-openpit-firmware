// TODO: Move to 'qt third party integration library'
#ifndef FORMAT_H
#define FORMAT_H

// std
#include <string_view>
// qt
#include <QString>

QT_BEGIN_NAMESPACE

inline std::string_view to_string_view(const QString& s) noexcept {
    return {
        reinterpret_cast<const char*>(s.utf16())
    ,   static_cast<size_t>(s.length())
    };
}

QT_END_NAMESPACE

#endif // FORMAT_H
