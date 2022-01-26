#ifndef COMPOSITIONROOTFACTORY_H
#define COMPOSITIONROOTFACTORY_H

// oss
#include <boost/di.hpp>

// App.Dmp.Presentation
#include "ViewWrapper.h"
// Modules.Sensors.Gps
#include "SerialRxGpsSensorPublisher.h"

class InjectorFactory {

public:
    InjectorFactory() = delete;

    static inline auto create() {
        return boost::di::make_injector(
            // Configure App.Dmp.Presentation
            boost::di::bind<ViewWrapper>()
                .in(boost::di::singleton)

            // Configure Modules.Sensors.Gps
        ,   boost::di::bind<IRxGpsSensorPublisher>()
                .to<SerialRxGpsSensorPublisher>()
                .in(boost::di::singleton)
        );
    }

};

#endif // COMPOSITIONROOTFACTORY_H
