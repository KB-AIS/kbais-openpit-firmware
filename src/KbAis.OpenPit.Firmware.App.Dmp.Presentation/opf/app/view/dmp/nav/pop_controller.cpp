#include "pop_controller.h"

void pop_controller::req_pop_wnd(int pop_wnd_idx) const {
    sub_pop_wnd_.get_subscriber().on_next(pop_wnd_idx);
}

rxcpp::observable<int> pop_controller::get_observable() const {
    return sub_pop_wnd_.get_observable();
}
