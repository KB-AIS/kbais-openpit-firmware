#ifndef AGTPCOMMANDSPARSER_H
#define AGTPCOMMANDSPARSER_H

// qt
#include <QIODevice>
#include <QVector>

#include "Handlers/IAgtpRequestHandler.h"

class AgtpRequestsParser {

public:
    AgtpRequestsParser() = delete;

    static QVector<AgtpRequest> parseFrame(QByteArray&& bytes);

    static QByteArray createFrame(const QVector<AgtpResponse>& responses);

};

#endif // AGTPCOMMANDSPARSER_H
