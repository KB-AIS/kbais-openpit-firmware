#ifndef SWOMPROTOCOLFORMATTER_H
#define SWOMPROTOCOLFORMATTER_H

// qt
#include <QByteArray>
#include <QString>
#include <QUuid>

#include "Persisting/Queries/SelectMessagesBatchesQry.h"

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

#endif // SWOMPROTOCOLFORMATTER_H
