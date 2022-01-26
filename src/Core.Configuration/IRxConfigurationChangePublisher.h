#ifndef IRXCONFIGURATIONCHANGEPUBLISHER_H
#define IRXCONFIGURATIONCHANGEPUBLISHER_H

// oss
#include <rxcpp/rx.hpp>

#include "Configuration.h"

class IRxConfigurationChangePublisher {

public:
    virtual ~IRxConfigurationChangePublisher() noexcept = default;

    virtual rxcpp::observable<Configuration> getChangeObservable(const QString& name) = 0;

};

#endif // IRXCONFIGURATIONCHANGEPUBLISHER_H
