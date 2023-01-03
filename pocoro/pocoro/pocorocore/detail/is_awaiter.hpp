//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_IS_AWAITER_HPP
#define POCORO_IS_AWAITER_HPP

#include <type_traits>
#include <coroutine>

namespace pocoro::detail
    {
        template<typename T>
        struct is_coroutine_handle
                : std::false_type
        {};

        template<typename PROMISE>
        struct is_coroutine_handle<std::coroutine_handle<PROMISE>>
                : std::true_type
        {};

        template<typename T>
        struct is_valid_await_suspend_return_value : std::disjunction<
                std::is_void<T>,
                std::is_same<T, bool>,
                is_coroutine_handle<T>>
        {};

        template<typename T, typename = std::void_t<>>
        struct is_awaiter : std::false_type {};

        template<typename T>
        struct is_awaiter<T, std::void_t<
                decltype(std::declval<T>().await_ready()),
                decltype(std::declval<T>().await_suspend(std::declval<std::coroutine_handle<>>())),
                decltype(std::declval<T>().await_resume())>> :
                std::conjunction<
                        std::is_constructible<bool, decltype(std::declval<T>().await_ready())>,
                        detail::is_valid_await_suspend_return_value<
                                decltype(std::declval<T>().await_suspend(std::declval<std::coroutine_handle<>>()))>>
        {};
    }
#endif //POCORO_IS_AWAITER_HPP
