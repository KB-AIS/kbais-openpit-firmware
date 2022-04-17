#include "NavController.h"

nav_controller::nav_controller() {

}

void
nav_controller::Navigate(int screenIdx) const {
    m_subjectReqestedDestId.get_subscriber().on_next(screenIdx);
}

rxcpp::observable<int>
nav_controller::GetObservableNavRequested() const {
    return m_subjectReqestedDestId.get_observable();
}
