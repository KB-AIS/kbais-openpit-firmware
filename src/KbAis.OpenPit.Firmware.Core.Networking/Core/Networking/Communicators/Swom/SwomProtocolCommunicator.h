#ifndef SWOMPROTOCOLCOMMUNICATOR_H
#define SWOMPROTOCOLCOMMUNICATOR_H

// qt
#include <QTimer>
#include <QUuid>

#include "RxQt.h"

#include "Core/Networking/Communicators/IProtocolCommunicator.h"

enum class SwomProtocolCommunicatorState {
    Authenticating,
    ReadyToSend,
    WaitAcknowledge,
    Stopped = -1,
};

class SwomProtocolCommunicator : public IProtocolCommunicator {

public:
    SwomProtocolCommunicator();

    ~SwomProtocolCommunicator();

    void InitCommunication(QIODevice& device);

    void StopCommunication();

private:
    SwomProtocolCommunicatorState m_currentState { SwomProtocolCommunicatorState::Stopped };

    rxcpp::composite_subscription m_subscriptions;

    QTimer m_tmAckTimeout;

    QTimer m_tmEnequeReccur;

    QUuid athUuid;

    std::vector<QUuid> m_recivedUuid;

    void OnReadyRead(QIODevice& device);

    void OnAckTimeout(QIODevice& device);

    void PerformAuthentication(QIODevice& device);

    void SendCollectedMessages(QIODevice& device);
};

#endif // SWOMPROTOCOLCOMMUNICATOR_H
