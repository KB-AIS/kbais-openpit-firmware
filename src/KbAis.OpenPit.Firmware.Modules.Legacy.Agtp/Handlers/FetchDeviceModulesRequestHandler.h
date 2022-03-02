#ifndef FETCHDEVICEMODULESREQUESTHANDLER_H
#define FETCHDEVICEMODULESREQUESTHANDLER_H


#include "IAgtpRequestHandler.h"

class FetchDeviceModulesRequestHandler : public IAgtpRequestHandler {

public:
    FetchDeviceModulesRequestHandler();

    AgtpResponse handle(const AgtpRequest& command) override;

    QString get_request_name() const override;
};

#endif // FETCHDEVICEMODULESREQUESTHANDLER_H
