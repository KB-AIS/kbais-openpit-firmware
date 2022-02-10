#ifndef SWOMPROTOCOLCOMMUNICATOR_H
#define SWOMPROTOCOLCOMMUNICATOR_H

// qt
#include <QTimer>
#include <QUuid>

#include "RxQt.h"

#include "Core/Networking/Communicators/IProtocolCommunicator.h"
#include "Core/Networking/Communicators/Swom/SwomProtocolFormatter.h"

struct SwomPacketMeta {
    QUuid uuid;

    SwomPacketType type;
};

enum SwomProtocolCommunicatorState {
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

    SwomPacketMeta m_sndAthPacket;

    std::vector<SwomPacketMeta> m_sndPackets;

    void OnReadyRead(QIODevice& device);

    void OnAckTimeout(QIODevice& device);

    void HandleAthReq(QIODevice& device);

    void HandleAthRsp(const std::vector<SwomAckPacket>& ackPackets);

    void SendCollectedMessages(QIODevice& device);
};

#endif // SWOMPROTOCOLCOMMUNICATOR_H
