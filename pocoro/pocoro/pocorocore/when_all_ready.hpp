//
// Created by quynh on 1/4/2023.
//

#ifndef POCORO_WHEN_ALL_READY_HPP
#define POCORO_WHEN_ALL_READY_HPP

#include "config.hpp"
#include "awaitable_traits.hpp"
#include "is_awaitable.hpp"

#include "detail/when_all_ready_awaitable.hpp"
#include "detail/when_all_task.hpp"
#include "detail/unwrap_reference.hpp"

#include <tuple>
#include <utility>
#include <vector>
#include <type_traits>

namespace pocoro
{
    template<
            typename... AWAITABLES,
            std::enable_if_t<std::conjunction_v<
                    is_awaitable<detail::unwrap_reference_t<std::remove_reference_t<AWAITABLES>>>...>, int> = 0>
    [[nodiscard]]
    POCORO_FORCE_INLINE auto when_all_ready(AWAITABLES&&... awaitables)
    {
        return detail::when_all_ready_awaitable<std::tuple<detail::when_all_task<
                typename awaitable_traits<detail::unwrap_reference_t<std::remove_reference_t<AWAITABLES>>>::await_result_t>...>>(
                std::make_tuple(detail::make_when_all_task(std::forward<AWAITABLES>(awaitables))...));
    }

    // TODO: Generalise this from vector<AWAITABLE> to arbitrary sequence of awaitable.

    template<
            typename AWAITABLE,
            typename RESULT = typename awaitable_traits<detail::unwrap_reference_t<AWAITABLE>>::await_result_t>
    [[nodiscard]] auto when_all_ready(std::vector<AWAITABLE> awaitables)
    {
        std::vector<detail::when_all_task<RESULT>> tasks;

        tasks.reserve(awaitables.size());

        for (auto& awaitable : awaitables)
        {
            tasks.emplace_back(detail::make_when_all_task(std::move(awaitable)));
        }

        return detail::when_all_ready_awaitable<std::vector<detail::when_all_task<RESULT>>>(
                std::move(tasks));
    }
}
#endif //POCORO_WHEN_ALL_READY_HPP
