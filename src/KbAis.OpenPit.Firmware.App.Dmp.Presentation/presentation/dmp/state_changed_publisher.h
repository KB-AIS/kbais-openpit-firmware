#ifndef STATE_CHANGED_PUBLISHER_H
#define STATE_CHANGED_PUBLISHER_H

#include <rxcpp/rx-observable.hpp>

enum class state_code_motioning {
    moving
,   staying
,   parking
};

class i_state_changed_publisher {

public:
    virtual ~i_state_changed_publisher() noexcept = default;

    virtual rxcpp::observable<state_code_motioning> get_observable(
        const rxcpp::observe_on_one_worker& coordination
    ) const = 0;

};

enum class state_code_loading : uint8_t {
    loading
,   loaded
,   unloading
,   empty
};

class i_state_loading_changed_publisher {

public:
    virtual ~i_state_loading_changed_publisher() noexcept = default;

    virtual rxcpp::observable<state_code_loading> get_observable(
        const rxcpp::observe_on_one_worker& coordination
    ) const = 0;

};

#endif // STATE_CHANGED_PUBLISHER_H
