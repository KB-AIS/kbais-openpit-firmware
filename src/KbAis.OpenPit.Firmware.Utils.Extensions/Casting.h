#ifndef CASTING_H
#define CASTING_H

// std
#include <functional>
#include <memory>
#include <type_traits>

namespace Utils::Casting {

/*!
 * \brief Cast value of base pointer to dervied and in case
 * of success perform provided action.
 */
template<typename TBase, typename TDerv>
void action_if(
    const std::shared_ptr<TDerv>& value,
    std::function<void(const std::shared_ptr<TBase>&)> action
) {
    auto casted_value = std::dynamic_pointer_cast<TBase>(value);
    if (!casted_value) return;

    action(casted_value);
}

}

#endif // CASTING_H
