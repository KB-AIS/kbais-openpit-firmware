#ifndef FETCHDEVICESPECREQUESTHANDLER_H
#define FETCHDEVICESPECREQUESTHANDLER_H

#include "IAgtpRequestHandler.h"

class FetchDeviceSpecRequestHandler : public IAgtpRequestHandler {

public:
    FetchDeviceSpecRequestHandler();

    AgtpResponse handle(const AgtpRequest &command) override;

    QString getRequestName() const override;
};

#endif // FETCHDEVICESPECREQUESTHANDLER_H
