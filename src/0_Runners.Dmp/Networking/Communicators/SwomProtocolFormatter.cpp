#include "SwomProtocolFormatter.h"

// qt
#include <QDataStream>
#include <QUuid>
// oss
#include <nlohmann/json.hpp>

// cfw::trdparty
#include "JsonQt/JsonQt.h"

using json = nlohmann::json;

QDataStream&
operator<<(QDataStream& out, const json& payload) {
    auto bytes = json::to_msgpack(payload);

    return out << QByteArray(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}

bool
SwomProtocolFormatter::decodeAckFrame(
    const QByteArray &bytes, AckFrame &outDecodedFrame, quint64 &outDecodedBytes
) const {
    return false;
}

QByteArray
SwomProtocolFormatter::encodeAthFrame(
    const QString& equipmentId, QUuid& outEncodedFrameUuid
) const {

}

QByteArray
SwomProtocolFormatter::encodeTelFrame(
    const QVector<MessagesBatchDto> &messageBatches, QUuid& outEncodedFrameUuid
) const {

}
