#ifndef SWOM_PROTOCOL_COMMUNICATOR_H
#define SWOM_PROTOCOL_COMMUNICATOR_H

// qt
#include <QIODevice>
#include <QObject>
#include <QTimer>
#include <QDebug>

#include "networking/communicators/base_protocol_communicator.h"
#include "networking/formatters/swom_protocol_formatter.h"
#include "persisting/commands/set_last_sent_messages_batch_id_command.h"
#include "persisting/queries/get_messages_batches_query.h"

enum class SwomProtocolCommunicatorState {
    Stoped,
    Authenticating,
    ReadyToSendData,
    WaitAcknowledge,
};

class SwomProtocolCommunicator : public BaseProtocolCommunicator {
    Q_OBJECT

public:
    SwomProtocolCommunicator();

    void beginCommunication(QIODevice& device);

    void interruptCommunication();

    void sendDataImmediatly();

    SwomProtocolCommunicatorState getCurrentState() const;

    Q_SIGNAL void notifySendDataImmediatlyRequired();

private:
    SwomProtocolCommunicatorState state;

    SwomProtocolFormatter formatter;

    GetMessagesBatchesQuery qryGetMessagesBatches;

    SetLastSentMessagesBatchIdCommand cmdSetLastSentMessagesBatchId;

    QTimer tWaitAcknowledge;

    QTimer tSendDataReccurently;

    QMetaObject::Connection cReadData;

    QMetaObject::Connection cSendDataReccurently;

    QMetaObject::Connection cSendDataImmediately;

    QMetaObject::Connection cWaitAcknowldege;

    void requestAuthentication(QIODevice& device);

};

#endif // SWOM_PROTOCOL_COMMUNICATOR_H
