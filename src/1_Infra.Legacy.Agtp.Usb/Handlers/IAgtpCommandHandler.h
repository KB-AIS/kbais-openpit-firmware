#ifndef IAGTPCOMMANDHANDLER_H
#define IAGTPCOMMANDHANDLER_H

// std
#include <vector>
// qt
#include <QString>

struct AgtpCommand {

    quint16 uid;

    QString command;

    quint8 type;

};

struct AgtpCommandResult {

    quint16 uid;

    QByteArray bytes;

    quint8 status;
};

class IAgtpCommandHandler {

public:
    virtual ~IAgtpCommandHandler() noexcept = default;

    virtual AgtpCommandResult handle(const AgtpCommand& command) = 0;

};

#endif // IAGTPCOMMANDHANDLER_H
