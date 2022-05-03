#ifndef DMPCONFIGURATIONINITIALIZER_H
#define DMPCONFIGURATIONINITIALIZER_H

#include "core/configuration/app_configuration_manager.h"

class app_configuration_initializer {

public:
    virtual ~app_configuration_initializer() noexcept = default;

};

class app_configuration_initializer_dmp : public app_configuration_initializer {

public:
    app_configuration_initializer_dmp(app_configuration_manager& manager);

};

#endif // DMPCONFIGURATIONINITIALIZER_H
