#ifndef IAGTPCOMMANDHANDLER_H
#define IAGTPCOMMANDHANDLER_H

// std
#include <vector>
// qt
#include <QString>

struct AgtpCommand {

    unsigned short uid;

    QString command;

    unsigned char type;

};

struct AgtpCommandResult {

    unsigned short uid;

    QByteArray bytes;

    unsigned char error;
};

class IAgtpCommandHandler {

public:
    virtual ~IAgtpCommandHandler() noexcept = default;

    virtual AgtpCommandResult handle(const AgtpCommand& command) = 0;

};

#endif // IAGTPCOMMANDHANDLER_H
