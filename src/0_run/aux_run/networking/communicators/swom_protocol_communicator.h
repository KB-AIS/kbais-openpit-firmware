#ifndef SWOM_PROTOCOL_COMMUNICATOR_H
#define SWOM_PROTOCOL_COMMUNICATOR_H

// qt
#include <QIODevice>
#include <QObject>
#include <QTimer>

#include "networking/communicators/base_protocol_communicator.h"
#include "networking/formatters/swom_protocol_formatter.h"
#include "persisting/commands/set_last_sent_messages_batch_id_command.h"
#include "persisting/queries/get_messages_batches_query.h"

enum class SwomProtocolCommunicatorState {
    Stoped,
    Authenticating,
    ReadyToSendData,
    WaitReplayOnDataSent,
};

class SwomProtocolCommunicator : public BaseProtocolCommunicator {
    Q_OBJECT

public:
    SwomProtocolCommunicator();

    void beginCommunication(QIODevice& device);

    void interruptCommunication();

    void sendMessage();

    SwomProtocolCommunicatorState getCurrentState() const;

    // TODO: Remove
    Q_SIGNAL void notifyNeedSend();

private:
    SwomProtocolCommunicatorState state;

    SwomProtocolFormatter formatter;

    GetMessagesBatchesQuery getMessagesBatchesQuery;

    SetLastSentMessagesBatchIdCommand setLastSentMessagesBatchIdCommand;

    QTimer timerWaitAcknowledge;

    QTimer timerSendMessagesBatches;

    QMetaObject::Connection connectionReadData;

    QMetaObject::Connection connectionSendData;

    QMetaObject::Connection connectionWaitReplay;

    void requestAuthentication(QIODevice& device);

};

#endif // SWOM_PROTOCOL_COMMUNICATOR_H
