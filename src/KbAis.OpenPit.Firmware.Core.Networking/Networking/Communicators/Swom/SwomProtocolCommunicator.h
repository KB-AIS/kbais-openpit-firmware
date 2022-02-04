#ifndef SWOMPROTOCOLCOMMUNICATOR_H
#define SWOMPROTOCOLCOMMUNICATOR_H

// qt
#include <QTimer>

#include "RxQt.h"

#include "Networking/Communicators/IProtocolCommunicator.h"

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
    rxcpp::composite_subscription m_subs;

    QTimer m_tEnequeReccur;
};

#endif // SWOMPROTOCOLCOMMUNICATOR_H
