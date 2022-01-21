#ifndef FETCHDEVICESPECCMDHANDLER_H
#define FETCHDEVICESPECCMDHANDLER_H

#include "IAgtpCommandHandler.h"

class FetchDeviceSpecCmdHandler : public IAgtpCommandHandler {

public:
    FetchDeviceSpecCmdHandler();

    AgtpCommandResult handle(const AgtpCommand &command) override;

    QString getCommand() const override;
};

#endif // FETCHDEVICESPECCMDHANDLER_H
