#ifndef SWOMPROTOCOLCOMMUNICATOR_H
#define SWOMPROTOCOLCOMMUNICATOR_H

// qt
#include <QTimer>
#include <QUuid>

#include "Core/Networking/Communicators/IProtocolCommunicator.h"
#include "Core/Networking/Communicators/Swom/SwomProtocolFormatter.h"
#include "Core/Persisting/Commands/UpdateSenderCmd.h"
#include "Core/Persisting/Queries/SelectMessagesBatchesQry.h"

enum SwomProtocolCommunicatorState {
    Authenticating
,   ReadyToSend
,   WaitAcknowledge
,   Stopped = -1
};

class SwomProtocolCommunicator : public IProtocolCommunicator {
    using SwomAthPacketMeta_t = QUuid;

    struct SwomTelPacketMeta { QUuid uuid; quint64 messageBatchId; };

public:
    SwomProtocolCommunicator();

    ~SwomProtocolCommunicator();

    void InitCommunication(QIODevice& device) override;

    void StopCommunication() override;

    NotificatorProtocolViolation_t GetObservableProtocolViolation() const override;

private:
    SwomProtocolCommunicatorState m_currentState { SwomProtocolCommunicatorState::Stopped };

    rxcpp::composite_subscription m_subscriptions;

    rxcpp::rxsub::subject<ProtocolViolationNotif> m_sbjNotificatorProtocolViolation;

    SwomAthPacketMeta_t m_sndAthPacket;

    std::vector<SwomTelPacketMeta> m_sndTelPackets;

    MessageBatchGetAllQry m_messageBatchGetAllHandler;

    MessageSenderUpdateCmd m_messageSenderUpdateCmd;

    QTimer m_tmSendMessageBatches;

    void OnReadyRead(QIODevice& device);

    void OnAckTimeout(QIODevice& device);

    void HandleAthReq(QIODevice& device);

    void HandleAthRsp(const std::vector<SwomAckPacket>& ackPackets);

    void HandleTelReq(QIODevice& device);

    void HandleTelRsp(const std::vector<SwomAckPacket>& ackPackets, QIODevice& device);

    void PublishProtocolViolation(const std::string& message);

};

#endif // SWOMPROTOCOLCOMMUNICATOR_H
