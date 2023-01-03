//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_ASYNC_LATCH_HPP
#define POCORO_ASYNC_LATCH_HPP

#include "async_manual_reset_event.hpp"

#include <atomic>
#include <cstdint>

namespace pocoro
{
    class async_latch
    {
    public:


        async_latch(std::ptrdiff_t initialCount) noexcept
                : m_count(initialCount)
                , m_event(initialCount <= 0)
        {}

        bool is_ready() const noexcept { return m_event.is_set(); }

        void count_down(std::ptrdiff_t n = 1) noexcept
        {
            if (m_count.fetch_sub(n, std::memory_order_acq_rel) <= n)
            {
                m_event.set();
            }
        }

        auto operator co_await() const noexcept
        {
            return m_event.operator co_await();
        }

    private:

        std::atomic<std::ptrdiff_t> m_count;
        async_manual_reset_event m_event;
    };
}

#endif //POCORO_ASYNC_LATCH_HPP
