#ifndef SWOM_PROTOCOL_COMMUNICATOR_H
#define SWOM_PROTOCOL_COMMUNICATOR_H

// qt
#include <QTimer>

#include "networking/communicators/base_protocol_communicator.h"
#include "networking/communicators/messages_batches_send_queue.h"
#include "networking/communicators/swom_protocol_formatter.h"
#include "persisting/commands/set_last_sent_messages_batch_id_command.h"
#include "persisting/queries/get_messages_batches_query.h"

enum class SwomProtocolCommunicatorState {
    Stoped = 0,
    Authenticating,
    ReadyToSendData,
    WaitAcknowledge,
};

struct SwomCurrentTelFrame {

};

class SwomProtocolCommunicator : public BaseProtocolCommunicator {
    Q_OBJECT
public:
    SwomProtocolCommunicator();

    void beginCommunication(QIODevice& device);

    void endCommunication();

    void requestSendImmediatly();

    SwomProtocolCommunicatorState state() const;

private:
    GetMessagesBatchesQuery getMessagesBatchesQuery;

    MessagesBatchesSendQueue sendQueue;

    QMetaObject::Connection cEnqueuReccurently;

    QMetaObject::Connection cReadData;

    QMetaObject::Connection cWaitAcknowldege;

    QTimer timerEnequeReccurently;

    QTimer timerWaitAcknowledge;

    SwomProtocolCommunicatorState currentState;

    SwomProtocolFormatter formatter;

    void requestAuthentication(QIODevice& device);

    Q_SLOT void enequeNextMessagesBatches();
};

#endif // SWOM_PROTOCOL_COMMUNICATOR_H
