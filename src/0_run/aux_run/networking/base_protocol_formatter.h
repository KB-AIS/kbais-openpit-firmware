#ifndef BASE_PROTOCOL_FORMATTER_H
#define BASE_PROTOCOL_FORMATTER_H

// qt
#include <QByteArray>

namespace kbais::cfw::networking {

class BaseProtocolFormatter {

public:
    QByteArray encode();

};

}

#endif // BASE_PROTOCOL_FORMATTER_H
