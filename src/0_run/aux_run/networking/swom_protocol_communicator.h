#ifndef SWOM_PROTOCOL_COMMUNICATOR_H
#define SWOM_PROTOCOL_COMMUNICATOR_H

// qt
#include <QIODevice>
#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>

#include "msgcaching/get_message_batches_query.h"
#include "networking/base_protocol_communicator.h"
#include "networking/base_protocol_formatter.h"

namespace KbAis::Cfw::Networking {

enum class SwomProtocolCommunicatorState {
    Stoped,
    Authenticating,
    ReadyToSendData,
    WaitReplayOnDataSent,
};

class SwomProtocolCommunicator : public BaseProtocolCommunicator {


public:
    SwomProtocolCommunicator();

    void start(QIODevice& device);

    void cease();

    void sendMessage();

    SwomProtocolCommunicatorState currentState() const;

private:
    bool fire;

    QMutex fireMtx;

    KbAis::Cfw::DatabaseCaching::GetMessageBatchesQueryHandler queryHandler;

    SwomProtocolCommunicatorState state;

    SwomProtocolFromatter formatter;

    QTimer timerWaitReplay;

    QTimer timerSendData;

    QMetaObject::Connection connectionReadData;

    QMetaObject::Connection connectionSendData;

    QMetaObject::Connection connectionWaitReplay;

    void requestAuthentication(QIODevice& device);

};

}

#endif // SWOM_PROTOCOL_COMMUNICATOR_H
