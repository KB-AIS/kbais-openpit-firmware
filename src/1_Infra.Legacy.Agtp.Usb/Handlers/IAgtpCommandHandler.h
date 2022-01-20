#ifndef IAGTPCOMMANDHANDLER_H
#define IAGTPCOMMANDHANDLER_H

// std
#include <vector>
// qt
#include <QString>

struct AgtpCommand {

    quint16 uid { 0 };

    QString command;

    quint8 type { 0 };

};

struct AgtpCommandResult {

    quint16 uid { 0 };

    QByteArray bytes;

    quint8 status { 0 };
};

class IAgtpCommandHandler {

public:
    virtual ~IAgtpCommandHandler() noexcept = default;

    virtual AgtpCommandResult handle(const AgtpCommand& command) = 0;

};

#endif // IAGTPCOMMANDHANDLER_H
