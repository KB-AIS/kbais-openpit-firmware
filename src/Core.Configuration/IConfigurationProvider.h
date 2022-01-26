#ifndef ICONFIGURATIONPROVIDER_H
#define ICONFIGURATIONPROVIDER_H

#include "Configuration.h"

class IConfigurationProvider {

public:
    virtual ~IConfigurationProvider() noexcept = default;

    virtual Configuration getConfiguration(const QString& name) = 0;

};

#endif // ICONFIGURATIONPROVIDER_H
