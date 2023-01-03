//
// Created by quynh on 1/4/2023.
//

#ifndef POCORO_IS_AWAITABLE_HPP
#define POCORO_IS_AWAITABLE_HPP
#include "detail/get_awaiter.hpp"

#include <type_traits>

namespace pocoro
{
    template<typename T, typename = std::void_t<>>
    struct is_awaitable : std::false_type {};

    template<typename T>
    struct is_awaitable<T, std::void_t<decltype(pocoro::detail::get_awaiter(std::declval<T>()))>>
            : std::true_type
    {};

    template<typename T>
    constexpr bool is_awaitable_v = is_awaitable<T>::value;
}

#endif //POCORO_IS_AWAITABLE_HPP
