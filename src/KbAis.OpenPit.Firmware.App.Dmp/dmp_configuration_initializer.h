#ifndef DMPCONFIGURATIONINITIALIZER_H
#define DMPCONFIGURATIONINITIALIZER_H

#include "ConfigurationsManager.h"

class ConfigurationInitializer {

public:
    virtual ~ConfigurationInitializer() noexcept = default;

};


class DmpConfigurationInitializer : public ConfigurationInitializer {

public:
    DmpConfigurationInitializer(ConfigurationManager& configuraiton_manager);

};

#endif // DMPCONFIGURATIONINITIALIZER_H
