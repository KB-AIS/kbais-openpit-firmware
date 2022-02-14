#ifndef MODULEFACTORYPRESENTATION_H
#define MODULEFACTORYPRESENTATION_H

// oss
#include <boost/di.hpp>

#include "App/Dmp/Presentation/BootstrapperPresentation.h"

inline auto CreateModulePresentation() noexcept {
    return boost::di::make_injector(
        boost::di::bind<NavController>.in(boost::di::singleton)
    ,   boost::di::bind<HostWindow>.in(boost::di::singleton)
    ,   boost::di::bind<DiagView>.in(boost::di::singleton)
    ,   boost::di::bind<MainView>.in(boost::di::singleton)
    ,   boost::di::bind<BootstrapperPresentation>.in(boost::di::singleton)
    );
}

#endif // MODULEFACTORYPRESENTATION_H
