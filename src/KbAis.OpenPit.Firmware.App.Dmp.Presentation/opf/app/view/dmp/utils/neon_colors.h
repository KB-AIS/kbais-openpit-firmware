#ifndef NEON_COLORS_H
#define NEON_COLORS_H

// qt
#include <QColor>

namespace opf::app::view::utils {

class colors {
    colors() = delete;

public:
    // TODO: Use constexpr after migration
    static inline const QColor neon_black  { 0x25, 0x25, 0x25 };
    static inline const QColor neon_blue   { 0x00, 0x0a, 0xff };
    static inline const QColor neon_green  { 0x00, 0xff, 0x28 };
    static inline const QColor neon_orange { 0xff, 0x7b, 0x02 };
    static inline const QColor neon_red    { 0xff, 0x02, 0x02 };
    static inline const QColor neon_white  { 0xff, 0xff, 0xff };
    static inline const QColor neon_yellow { 0xff, 0xe4, 0x00 };

};

}

#endif // NEON_COLORS_H
