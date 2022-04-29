#ifndef POP_CONTROLLER_H
#define POP_CONTROLLER_H

#include <rxcpp/rx.hpp>

class pop_controller {

    rxcpp::rxsub::subject<int> sub_pop_wnd_;

public:
    void req_pop_wnd(int pop_wnd_idx) const;

    rxcpp::observable<int> get_observable() const;

};

#endif // POP_CONTROLLER_H
