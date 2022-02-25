#ifndef AGTPPROTOCOLPARESER_H
#define AGTPPROTOCOLPARESER_H

// qt
#include <QIODevice>
#include <QVector>
// oss
#include <nonstd/expected.hpp>

#include "Handlers/IAgtpRequestHandler.h"

class AgtpProtocolParser {

public:
    AgtpProtocolParser() = delete;

    static nonstd::expected<std::vector<AgtpRequest>, bool> parseRequest(QByteArray&& bytes);

    static QByteArray createResponse(const std::vector<AgtpResponse>& responses);

};

#endif // AGTPPROTOCOLPARESER_H
