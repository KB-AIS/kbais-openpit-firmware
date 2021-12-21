#ifndef BASE_PROTOCOL_FORMATTER_H
#define BASE_PROTOCOL_FORMATTER_H

// qt
#include <QByteArray>
#include <QString>

class BaseProtocolFormatter {

public:
    virtual QByteArray encodeAuthPackage(const QString& equipmentId) = 0;

};

#endif // BASE_PROTOCOL_FORMATTER_H
