#include "ntf_controller.h"

void ntf_controller::req_ntf(const ntf_prms&& prms) const {
    sub_notification_.get_subscriber().on_next(prms);
}

rxcpp::observable<ntf_prms> ntf_controller::get_observable() const {
    return sub_notification_.get_observable();
}
