#include "NavigationEmmiter.h"

NavEmmiter::NavEmmiter() {

}

void
NavEmmiter::RequestNavTo(int screenIdx) const {
    m_subRequestedScreenIdx.get_subscriber().on_next(screenIdx);
}

rxcpp::observable<int>
NavEmmiter::GetObservableNavRequested() const {
    return m_subRequestedScreenIdx.get_observable();
}
