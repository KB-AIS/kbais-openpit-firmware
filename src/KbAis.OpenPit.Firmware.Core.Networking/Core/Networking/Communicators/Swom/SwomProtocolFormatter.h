#ifndef SWOMPROTOCOLFORMATTER_H
#define SWOMPROTOCOLFORMATTER_H

// qt
#include <QByteArray>
#include <QUuid>

#include <Core/Persisting/Queries/Dtos.h>

enum class SwomFrameType : quint8 {
    Ath = 0x01
,   Tel = 0x02
,   Ack = 0xFF
};

class SwomProtocolFormatter {

public:
    SwomProtocolFormatter() = delete;

    static QByteArray EncodeAthFrame(const QUuid& uuid, const QString& equipmentId);

    static QByteArray EncodeTelFrame(const QUuid& uuid, const MessagesBatchDto& messageBatch);

};

#endif // SWOMPROTOCOLFORMATTER_H
