#ifndef BASE_PROTOCOL_FORMATTER_H
#define BASE_PROTOCOL_FORMATTER_H

// qt
#include <QByteArray>
#include <QIODevice>
#include <QObject>
#include <QString>
#include <QUuid>

namespace KbAis::Cfw::Networking {

enum class DataType : quint8 {
    Ath = 0x00,
    Tel = 0x01,
    Ack = 0xFF,
};

struct AthData {

    QString equipmentId;

};

struct AckData {

    QUuid uuid;

    quint8 result;

};

struct TelData {

};

class BaseProtocolFormatter {

public:
    virtual QByteArray encodeAuthPackage(const QString& equipmentId) = 0;

};

class SwomProtocolFromatter : public BaseProtocolFormatter {

public:
    QByteArray encodeAuthPackage(const QString &equipmentId) override;

};

}

#endif // BASE_PROTOCOL_FORMATTER_H
