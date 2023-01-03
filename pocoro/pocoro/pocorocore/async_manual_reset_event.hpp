//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_ASYNC_MANUAL_RESET_EVENT_HPP
#define POCORO_ASYNC_MANUAL_RESET_EVENT_HPP

#include <coroutine>
#include <atomic>
#include <cstdint>

namespace pocoro
{
    class async_manual_reset_event_operation;

    class async_manual_reset_event
    {
    public:

        async_manual_reset_event(bool initiallySet = false) noexcept;

        ~async_manual_reset_event();

        async_manual_reset_event_operation operator co_await() const noexcept;

        /// Query if the event is currently in the 'set' state.
        bool is_set() const noexcept;

        void set() noexcept;

        void reset() noexcept;

    private:

        friend class async_manual_reset_event_operation;
        mutable std::atomic<void*> m_state;

    };

    class async_manual_reset_event_operation
    {
    public:

        explicit async_manual_reset_event_operation(const async_manual_reset_event& event) noexcept;

        bool await_ready() const noexcept;
        bool await_suspend(std::coroutine_handle<> awaiter) noexcept;
        void await_resume() const noexcept {}

    private:

        friend class async_manual_reset_event;

        const async_manual_reset_event& m_event;
        async_manual_reset_event_operation* m_next;
        std::coroutine_handle<> m_awaiter;

    };
}

#endif //POCORO_ASYNC_MANUAL_RESET_EVENT_HPP
