#include "nav_controller.h"

void nav_controller::nav_to(const int screen_id) const {
    subject_screen_id_.get_subscriber().on_next(screen_id);
}

rxcpp::observable<int> nav_controller::get_observable() const {
    return subject_screen_id_.get_observable();
}
