#ifndef SWOMPROTOCOLCOMMUNICATOR_H
#define SWOMPROTOCOLCOMMUNICATOR_H

// qt
#include <QTimer>

#include "RxQt.h"

#include "Core/Networking/Communicators/IProtocolCommunicator.h"

enum class SwomProtocolCommunicatorState {
    Authenticating,
    ReadyToSend,
    WaitAcknowledge,

    Stoped = -1,
};

class SwomProtocolCommunicator : public IProtocolCommunicator {

public:
    SwomProtocolCommunicator();

    ~SwomProtocolCommunicator();

    void InitCommunication(QIODevice &device);

    void StopCommunication();

private:
    SwomProtocolCommunicatorState m_currentState;

    rxcpp::composite_subscription m_subs;

    QTimer m_tEnequeReccur;

    void PerformAuthenticationRequest(QIODevice& device);
};

#endif // SWOMPROTOCOLCOMMUNICATOR_H
