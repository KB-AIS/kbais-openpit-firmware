#ifndef SWOM_PROTOCOL_FORMATTER_H
#define SWOM_PROTOCOL_FORMATTER_H

// qt
#include <QByteArray>
#include <QString>
#include <QUuid>

#include "networking/formatters/base_protocol_formatter.h"

enum class SwomFrameType : quint8 {
    Ath = 0x00,
    Tel = 0x01,
    Ack = 0xFF,
};

class SwomProtocolFormatter : public BaseProtocolFormatter {

public:
    QByteArray encodeAuthPackage(const QString& equipmentId) override;

};

#endif // SWOM_PROTOCOL_FORMATTER_H
