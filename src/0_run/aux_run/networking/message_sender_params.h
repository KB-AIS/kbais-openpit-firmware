#ifndef MESSAGE_SENDER_PARAMS_H
#define MESSAGE_SENDER_PARAMS_H

#include "networking/base_protocol_formatter.h"
// Qt
#include <QHostAddress>

namespace kbais::cfw::networking {

struct MessageSenderConfiguration {

    QString host;

    quint16 port;

    std::chrono::milliseconds requestInterval;

    BaseProtocolFormatter fmt;

};

}

#endif // MESSAGE_SENDER_PARAMS_H
