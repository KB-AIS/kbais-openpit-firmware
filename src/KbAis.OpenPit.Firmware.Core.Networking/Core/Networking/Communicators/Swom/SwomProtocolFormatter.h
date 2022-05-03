#ifndef SWOMPROTOCOLFORMATTER_H
#define SWOMPROTOCOLFORMATTER_H

// std
#include <variant>
// qt
#include <QByteArray>
#include <QUuid>
// oss
#include <nonstd/expected.hpp>

#include "Core/Persisting/Queries/Dtos.h"
#include "qt_json.h"

enum class SwomPacketType : quint8 {
    Ath = 0x01
,   Tel = 0x02
,   Ack = 0xFF
};

struct SwomAckPacket {

    enum AckResultCode : quint8 {
        Ok = 0x00
    ,   Error = 0xFF
    };

    QUuid AckUuid;

    AckResultCode Result;

};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SwomAckPacket, AckUuid, Result);

using DecodeAckFrameResult_t = nonstd::expected<std::vector<SwomAckPacket>, std::string>;

class SwomProtocolFormatter {

public:
    SwomProtocolFormatter() = delete;

    static DecodeAckFrameResult_t DecodeAckFrame(QByteArray& encodedFrame);

    static QByteArray EncodeAthPacket(const QUuid& uuid, const QString& equipmentId);

    static QByteArray EncodeTelPacket(const QUuid& uuid, const MessagesBatchDto& messageBatch);

    static QByteArray EncodeFrame(const QByteArray& encodedPackets);

};

#endif // SWOMPROTOCOLFORMATTER_H
