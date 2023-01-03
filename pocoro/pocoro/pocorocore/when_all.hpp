//
// Created by quynh on 1/4/2023.
//

#ifndef POCORO_WHEN_ALL_HPP
#define POCORO_WHEN_ALL_HPP

#include "when_all_ready.hpp"
#include "awaitable_traits.hpp"
#include "is_awaitable.hpp"
#include "fmap.hpp"

#include "detail/unwrap_reference.hpp"

#include <tuple>
#include <functional>
#include <utility>
#include <vector>
#include <type_traits>
#include <cassert>

namespace pocoro
{
    //////////
    // Variadic when_all()

    template<
            typename... AWAITABLES,
            std::enable_if_t<
                    std::conjunction_v<is_awaitable<detail::unwrap_reference_t<std::remove_reference_t<AWAITABLES>>>...>,
    int> = 0>
    [[nodiscard]] auto when_all(AWAITABLES&&... awaitables)
    {
        return fmap([](auto&& taskTuple)
                    {
                        return std::apply([](auto&&... tasks) {
                            return std::make_tuple(static_cast<decltype(tasks)>(tasks).non_void_result()...);
                        }, static_cast<decltype(taskTuple)>(taskTuple));
                    }, when_all_ready(std::forward<AWAITABLES>(awaitables)...));
    }

    //////////
    // when_all() with vector of awaitable

    template<
            typename AWAITABLE,
            typename RESULT = typename awaitable_traits<detail::unwrap_reference_t<AWAITABLE>>::await_result_t,
    std::enable_if_t<std::is_void_v<RESULT>, int> = 0>
                                                    [[nodiscard]]
    auto when_all(std::vector<AWAITABLE> awaitables)
    {
        return fmap([](auto&& taskVector) {
            for (auto& task : taskVector)
            {
                task.result();
            }
        }, when_all_ready(std::move(awaitables)));
    }

    template<
            typename AWAITABLE,
            typename RESULT = typename awaitable_traits<detail::unwrap_reference_t<AWAITABLE>>::await_result_t,
    std::enable_if_t<!std::is_void_v<RESULT>, int> = 0>
                                                     [[nodiscard]]
    auto when_all(std::vector<AWAITABLE> awaitables)
    {
        using result_t = std::conditional_t<
                std::is_lvalue_reference_v<RESULT>,
                std::reference_wrapper<std::remove_reference_t<RESULT>>,
                std::remove_reference_t<RESULT>>;

        return fmap([](auto&& taskVector) {
            std::vector<result_t> results;
            results.reserve(taskVector.size());
            for (auto& task : taskVector)
            {
                if constexpr (std::is_rvalue_reference_v<decltype(taskVector)>)
                {
                    results.emplace_back(std::move(task).result());
                }
                else
                {
                    results.emplace_back(task.result());
                }
            }
            return results;
        }, when_all_ready(std::move(awaitables)));
    }
}

#endif //POCORO_WHEN_ALL_HPP
