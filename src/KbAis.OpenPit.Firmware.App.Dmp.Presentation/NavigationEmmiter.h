#ifndef NAVIGATIONEMMITER_H
#define NAVIGATIONEMMITER_H

#include <rxcpp/rx.hpp>

class NavEmmiter {

public:
    NavEmmiter();

    void RequestNavTo(int screenIdx) const;

    rxcpp::observable<int> GetObservableNavRequested() const;

private:
    rxcpp::rxsub::subject<int> m_subRequestedScreenIdx;

};

#endif // NAVIGATIONEMMITER_H
