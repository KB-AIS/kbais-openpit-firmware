#ifndef AGTPPROTOCOLSERIALIZER_H
#define AGTPPROTOCOLSERIALIZER_H

// qt
#include <QIODevice>
#include <QVector>

#include "Handlers/IAgtpCommandHandler.h"

class AgtpProtocolSerializer {

public:
    AgtpProtocolSerializer() = delete;

    static QVector<AgtpCommand> deserialize(QByteArray&& bytes);

    static QByteArray serialize(const QVector<AgtpCommandResult>& results);

private:
    static void parsePacket();

};

#endif // AGTPPROTOCOLSERIALIZER_H
