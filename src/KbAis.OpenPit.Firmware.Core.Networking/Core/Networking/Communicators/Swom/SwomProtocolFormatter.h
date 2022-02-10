#ifndef SWOMPROTOCOLFORMATTER_H
#define SWOMPROTOCOLFORMATTER_H

// qt
#include <QByteArray>
#include <QUuid>
// oss
#include <nonstd/expected.hpp>

#include <Core/Persisting/Queries/Dtos.h>

enum class SwomPacketType : quint8 {
    Ath = 0x01
,   Tel = 0x02
,   Ack = 0xFF
};

enum class SwomProtocolFormatterError {
    DecodeFrameIsTooShort
,   DecodeUnsupportedFrame
,   DecodeChecksumInvalid
};

using DecodeAckFrameResult_t = nonstd::expected<std::vector<QUuid>, SwomProtocolFormatterError>;

class SwomProtocolFormatter {

public:
    SwomProtocolFormatter() = delete;

    static DecodeAckFrameResult_t DecodeAckFrame(QByteArray& encodedFrame);

    static QByteArray EncodeAthPacket(const QUuid& uuid, const QString& equipmentId);

    static QByteArray EncodeTelPacket(const QUuid& uuid, const MessagesBatchDto& messageBatch);

    static QByteArray EncodeFrame(const QByteArray& encodedPackets);

};

#endif // SWOMPROTOCOLFORMATTER_H
