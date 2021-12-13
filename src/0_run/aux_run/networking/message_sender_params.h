#ifndef MESSAGE_SENDER_PARAMS_H
#define MESSAGE_SENDER_PARAMS_H

#include "networking/base_protocol_formatter.h"
// Qt
#include <QHostAddress>

namespace kbais::cfw::networking {

struct MessageSenderConfiguration {

    const QString host;

    const quint16 port;

    const std::chrono::milliseconds requestInterval;

    const BaseProtocolFormatter& fmt;

};

}

#endif // MESSAGE_SENDER_PARAMS_H
