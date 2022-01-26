#ifndef IAGTPREQUESTSMEDIATOR_H
#define IAGTPREQUESTSMEDIATOR_H

#include "Handlers/IAgtpRequestHandler.h"

class IAgtpRequestsMediator {

public:
    ~IAgtpRequestsMediator() noexcept = default;

    virtual AgtpResponse handle(const AgtpRequest& request) const = 0;

};

#endif // IAGTPREQUESTSMEDIATOR_H
