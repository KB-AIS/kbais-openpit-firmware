#ifndef AGTPCOMMANDSPARSER_H
#define AGTPCOMMANDSPARSER_H

// qt
#include <QIODevice>
#include <QVector>

#include "Handlers/IAgtpCommandHandler.h"

class AgtpCommandsParser {

public:
    AgtpCommandsParser() = delete;

    static QVector<AgtpCommand> parseFrame(QByteArray&& bytes);

    static QByteArray createFrame(const QVector<AgtpCommandResult>& commandResults);

};

#endif // AGTPCOMMANDSPARSER_H
