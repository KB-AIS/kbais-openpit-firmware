#ifndef CONFIGSERIALSREQUESTHANDLER_H
#define CONFIGSERIALSREQUESTHANDLER_H

#include "IAgtpRequestHandler.h"

class ConfigSerialsRequestHandler : public IAgtpRequestHandler {

public:
    ConfigSerialsRequestHandler();

    AgtpResponse handle(const AgtpRequest &request);

    QString get_request_name() const;
};

class ConfigSerialsTklsRequestHandler : public IAgtpRequestHandler {
    AgtpResponse handle(const AgtpRequest &request);

    QString get_request_name() const;
};

class ConfigSerialsTpmsRequestHandler : public IAgtpRequestHandler {
    AgtpResponse handle(const AgtpRequest &request);

    QString get_request_name() const;
};

class ConfigSerialsLcsRequestHandler : public IAgtpRequestHandler {
    AgtpResponse handle(const AgtpRequest &request);

    QString get_request_name() const;
};

class ConfigSerialsStscpRequestHandler : public IAgtpRequestHandler {
    AgtpResponse handle(const AgtpRequest &request);

    QString get_request_name() const;
};

class ConfigSerialsTkcrRequestHandler : public IAgtpRequestHandler {
    AgtpResponse handle(const AgtpRequest &request);

    QString get_request_name() const;
};

#endif // CONFIGSERIALSREQUESTHANDLER_H
