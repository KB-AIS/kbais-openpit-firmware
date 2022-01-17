#ifndef SWOMPROTOCOLCOMMUNICATOR_H
#define SWOMPROTOCOLCOMMUNICATOR_H

// qt
#include <QTimer>

#include "Networking/Communicators/IProtocolCommunicator.h"
#include "Networking/Communicators/MessagesBatchesSendQueue.h"
#include "Networking/Communicators/SwomProtocolFormatter.h"
#include "Persisting/Commands/UpdateSenderCmd.h"

enum class SwomProtocolCommunicatorState {
    Stoped = 0,
    Authenticating,
    ReadyToSendData,
    WaitAcknowledge,
};

class SwomProtocolCommunicator : public IProtocolCommunicator {
    Q_OBJECT

public:
    SwomProtocolCommunicator();

    void beginCommunication(QIODevice& device);

    void endCommunication();

    void requestSendImmediatly();

    SwomProtocolCommunicatorState state() const;

private:
    SelectMessagesBatchesQry getMessagesBatchesQuery;

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

#endif // SWOMPROTOCOLCOMMUNICATOR_H
