#ifndef FETCHDEVICEMODULESCMDHANDLER_H
#define FETCHDEVICEMODULESCMDHANDLER_H


#include "IAgtpRequestHandler.h"

class FetchDeviceModulesCmdHandler : public IAgtpRequestHandler {

public:
    FetchDeviceModulesCmdHandler();

    AgtpResponse handle(const AgtpRequest& command) override;

    QString getRequestName() const override;
};

#endif // FETCHDEVICEMODULESCMDHANDLER_H
