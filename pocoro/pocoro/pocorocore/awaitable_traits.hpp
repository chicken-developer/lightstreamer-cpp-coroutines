//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_AWAITABLE_TRAITS_HPP
#define POCORO_AWAITABLE_TRAITS_HPP

#include "detail/get_awaiter.hpp"

#include <type_traits>

namespace pocoro
{
    template<typename T, typename = void>
    struct awaitable_traits
    {};

    template<typename T>
    struct awaitable_traits<T, std::void_t<decltype(pocoro::detail::get_awaiter(std::declval<T>()))>>
    {
        using awaiter_t = decltype(pocoro::detail::get_awaiter(std::declval<T>()));

        using await_result_t = decltype(std::declval<awaiter_t>().await_resume());
    };
}

#endif //POCORO_AWAITABLE_TRAITS_HPP
