#ifndef SWOM_PROTOCOL_COMMUNICATOR_H
#define SWOM_PROTOCOL_COMMUNICATOR_H

// qt
#include <QIODevice>
#include <QObject>
#include <QTimer>

#include "database/queries/get_messages_batches_query.h"
#include "networking/communicators/base_protocol_communicator.h"
#include "networking/formatters/swom_protocol_formatter.h"

enum class SwomProtocolCommunicatorState {
    Stoped,
    Authenticating,
    ReadyToSendData,
    WaitReplayOnDataSent,
};

class SwomProtocolCommunicator : public BaseProtocolCommunicator {

public:
    SwomProtocolCommunicator();

    void beginCommunication(QIODevice& device);

    void interruptCommunication();

    void sendMessage();

    SwomProtocolCommunicatorState currentState() const;

private:
    SwomProtocolCommunicatorState state;

    GetMessagesBatchesQuery getMessagesBatchesQuery;

    SwomProtocolFormatter formatter;

    QTimer timerWaitReplay;

    QTimer timerSendData;

    QMetaObject::Connection connectionReadData;

    QMetaObject::Connection connectionSendData;

    QMetaObject::Connection connectionWaitReplay;

    void requestAuthentication(QIODevice& device);

};

#endif // SWOM_PROTOCOL_COMMUNICATOR_H
