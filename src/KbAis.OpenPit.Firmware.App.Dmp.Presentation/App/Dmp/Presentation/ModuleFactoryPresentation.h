#ifndef MODULEFACTORYPRESENTATION_H
#define MODULEFACTORYPRESENTATION_H

// oss
#include <boost/di.hpp>

#include "App/Dmp/Presentation/BootstrapperPresentation.h"

inline auto create_module_presentation_dmp() noexcept {
    return boost::di::make_injector(
        boost::di::bind<nav_controller>.in(boost::di::singleton)
    ,   boost::di::bind<HostWindow>.in(boost::di::singleton)
    ,   boost::di::bind<DiagView>.in(boost::di::singleton)
    ,   boost::di::bind<i_main_view>.to<main_view>().in(boost::di::singleton)
    ,   boost::di::bind<BootstrapperPresentation>.in(boost::di::singleton)
    );
}

#endif // MODULEFACTORYPRESENTATION_H
