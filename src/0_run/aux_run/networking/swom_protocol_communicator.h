#ifndef SWOM_PROTOCOL_COMMUNICATOR_H
#define SWOM_PROTOCOL_COMMUNICATOR_H

// qt
#include <QIODevice>
#include <QObject>

#include "networking/base_protocol_communicator.h"
#include "networking/base_protocol_formatter.h"

namespace KbAis::Cfw::Networking {

class SwomProtocolCommunicator : public BaseProtocolCommunicator {

public:
    void startCommunication(QIODevice& device);

    void ceaseCommunication();

private:
    SwomProtocolFromatter formatter;

    QMetaObject::Connection cReadyRead;

};

}

#endif // SWOM_PROTOCOL_COMMUNICATOR_H
