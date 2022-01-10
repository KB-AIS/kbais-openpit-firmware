#ifndef SWOM_PROTOCOL_FORMATTER_H
#define SWOM_PROTOCOL_FORMATTER_H

// qt
#include <QByteArray>
#include <QString>
#include <QUuid>

#include "persisting/queries/get_messages_batches_query.h"

enum class SwomFrameType : quint8 {
    Ath = 0x00,
    Tel = 0x01,
    Ack = 0xFF,
};

struct AckFrame {
    /*!
     * Identifier of an acknowledged frame.
     */
    QUuid uuid;

    quint8 status;
};

struct SwomProtocolFormatter {
    bool decodeAckFrame(
        const QByteArray& bytes, AckFrame& outDecodedFrame, quint64& outDecodedBytes
    ) const;

    QByteArray encodeAthFrame(
        const QString& equipmentId, QUuid& outEncodedFrameUuid
    ) const;

    QByteArray encodeTelFrame(
        const QVector<MessagesBatchDto>& messageBatches, QUuid& outEncodedFrameUuid
    ) const;
};

#endif // SWOM_PROTOCOL_FORMATTER_H
