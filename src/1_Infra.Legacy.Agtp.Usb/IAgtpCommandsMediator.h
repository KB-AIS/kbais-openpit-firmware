#ifndef IAGTPCOMMANDSMEDIATOR_H
#define IAGTPCOMMANDSMEDIATOR_H

#include "Handlers/IAgtpRequestHandler.h"

class IAgtpCommandsMediator {

public:
    ~IAgtpCommandsMediator() noexcept = default;

    virtual AgtpResponse handle(const AgtpRequest& request) const = 0;

};

#endif // IAGTPCOMMANDSMEDIATOR_H
