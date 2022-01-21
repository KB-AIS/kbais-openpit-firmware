#ifndef FETCHDEVICEMODULESCMDHANDLER_H
#define FETCHDEVICEMODULESCMDHANDLER_H


#include "IAgtpCommandHandler.h"

class FetchDeviceModulesCmdHandler : public IAgtpCommandHandler {

public:
    FetchDeviceModulesCmdHandler();

    AgtpCommandResult handle(const AgtpCommand& command) override;

    QString getCommand() const override;
};

#endif // FETCHDEVICEMODULESCMDHANDLER_H
