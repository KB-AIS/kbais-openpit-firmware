#include "NavController.h"

NavController::NavController() {

}

void
NavController::Navigate(int screenIdx) const {
    m_subjectReqestedDestId.get_subscriber().on_next(screenIdx);
}

rxcpp::observable<int>
NavController::GetObservableNavRequested() const {
    return m_subjectReqestedDestId.get_observable();
}
