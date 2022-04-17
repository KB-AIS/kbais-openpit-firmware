#ifndef SCENARIO_EXECUTOR_H
#define SCENARIO_EXECUTOR_H

// oss
#include <rxcpp/rx.hpp>

namespace rx {

using namespace rxcpp;
using namespace rxcpp::operators;
using namespace rxcpp::sources;
using namespace rxcpp::util;

}

class i_scenario_executor {

public:
    virtual ~i_scenario_executor() noexcept = default;

    virtual void setup_scenario(const rxcpp::observe_on_one_worker& coordination) = 0;

    virtual void start_scenario() = 0;

};

#endif // SCENARIO_EXECUTOR_H
