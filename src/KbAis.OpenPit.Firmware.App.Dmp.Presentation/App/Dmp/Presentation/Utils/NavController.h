#ifndef NAVCONTROLLER_H
#define NAVCONTROLLER_H

#include <rxcpp/rx.hpp>

class NavController {

public:
    NavController();

    void Navigate(int destId) const;

    rxcpp::observable<int> GetObservableNavRequested() const;

private:
    rxcpp::rxsub::subject<int> m_subjectReqestedDestId;

};

#endif // NAVCONTROLLER_H
