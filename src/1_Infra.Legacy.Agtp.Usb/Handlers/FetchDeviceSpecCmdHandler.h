#ifndef FETCHDEVICESPECCMDHANDLER_H
#define FETCHDEVICESPECCMDHANDLER_H

#include "IAgtpRequestHandler.h"

class FetchDeviceSpecCmdHandler : public IAgtpRequestHandler {

public:
    FetchDeviceSpecCmdHandler();

    AgtpResponse handle(const AgtpRequest &command) override;

    QString getRequestName() const override;
};

#endif // FETCHDEVICESPECCMDHANDLER_H
