#include "nav_controller.h"

void nav_controller::nav_to(const int view_idx) const {
    sub_idx_.get_subscriber().on_next(view_idx);
}

rxcpp::observable<int> nav_controller::get_observable() const {
    return sub_idx_.get_observable();
}
