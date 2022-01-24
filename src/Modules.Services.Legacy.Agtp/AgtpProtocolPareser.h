#ifndef AGTPPROTOCOLPARESER_H
#define AGTPPROTOCOLPARESER_H

// qt
#include <QIODevice>
#include <QVector>

#include "Handlers/IAgtpRequestHandler.h"

class AgtpProtocolParser {

public:
    AgtpProtocolParser() = delete;

    static QVector<AgtpRequest> parseRequest(QByteArray&& bytes);

    static QByteArray createResponse(const QVector<AgtpResponse>& responses);

};

#endif // AGTPPROTOCOLPARESER_H
