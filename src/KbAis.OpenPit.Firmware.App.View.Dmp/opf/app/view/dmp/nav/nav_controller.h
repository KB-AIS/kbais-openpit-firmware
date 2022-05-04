#ifndef NAV_CONTROLLER_H
#define NAV_CONTROLLER_H

// oss
#include <rxcpp/rx.hpp>

class nav_controller {

    rxcpp::rxsub::subject<int> sub_idx_;

public:
    void nav_to(const int view_idx) const;

    rxcpp::observable<int> get_observable() const;

};

#endif // NAV_CONTROLLER_H
