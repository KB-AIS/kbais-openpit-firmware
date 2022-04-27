#ifndef MODULE_FACTORY_APP_VIEW_DMP_H
#define MODULE_FACTORY_APP_VIEW_DMP_H

// oss
#include <boost/di.hpp>

#include "opf/app/view/dmp/runner_app_view_dmp.h"

inline auto create_app_view_dmp() noexcept {
    return boost::di::make_injector(
        boost::di::bind<nav_controller>
            .in(boost::di::singleton)
    ,   boost::di::bind<host_window>
            .in(boost::di::singleton)
    ,   boost::di::bind<diag_view>
            .in(boost::di::singleton)
    ,   boost::di::bind<info_view>
            .in(boost::di::singleton)
    ,   boost::di::bind<i_main_view>
            .to<main_view>().in(boost::di::singleton)
    ,   boost::di::bind<runner_app_view_dmp>
            .in(boost::di::singleton)
    );
}

#endif // MODULE_FACTORY_APP_VIEW_DMP_H
