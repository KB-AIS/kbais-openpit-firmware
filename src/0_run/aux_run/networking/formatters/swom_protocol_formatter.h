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

class SwomProtocolFormatter {

public:
    QByteArray encodeAthFrame(const QString& equipmentId) const;

    QByteArray encodeTelFrame(const QList<MessagesBatchDto>& messageBatches) const;

};

#endif // SWOM_PROTOCOL_FORMATTER_H
