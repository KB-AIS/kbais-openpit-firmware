#ifndef SWOMPROTOCOLFORMATTER_H
#define SWOMPROTOCOLFORMATTER_H

// qt
#include <QByteArray>

#include <Core/Persisting/Queries/Dtos.h>

class SwomProtocolFormatter {

public:
    SwomProtocolFormatter() = delete;

    static QByteArray EncodeAckFrame(const QString& clientId);

    static QByteArray EncodeTelFrame(const QVector<MessagesBatchDto>& messageBatches);

};

#endif // SWOMPROTOCOLFORMATTER_H
