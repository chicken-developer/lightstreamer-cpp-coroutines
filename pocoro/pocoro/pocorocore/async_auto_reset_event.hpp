//
// Created by quynh on 1/3/2023.
//

#include <coroutine>
#include <atomic>
#include <cstdint>

#ifndef POCORO_ASYNC_AUTO_RESET_EVENT_HPP
#define POCORO_ASYNC_AUTO_RESET_EVENT_HPP

namespace pocoro
{
    class async_auto_reset_event_operation;

    class async_auto_reset_event
    {
    public:

        /// Initialise the event to either 'set' or 'not set' state.
        async_auto_reset_event(bool initiallySet = false) noexcept;

        ~async_auto_reset_event();

        async_auto_reset_event_operation operator co_await() const noexcept;

        void set() noexcept;

        void reset() noexcept;

    private:

        friend class async_auto_reset_event_operation;

        void resume_waiters(std::uint64_t initialState) const noexcept;


        mutable std::atomic<std::uint64_t> m_state;

        mutable std::atomic<async_auto_reset_event_operation*> m_newWaiters;

        mutable async_auto_reset_event_operation* m_waiters;

    };

    class async_auto_reset_event_operation
    {
    public:

        async_auto_reset_event_operation() noexcept;

        explicit async_auto_reset_event_operation(const async_auto_reset_event& event) noexcept;

        async_auto_reset_event_operation(const async_auto_reset_event_operation& other) noexcept;

        bool await_ready() const noexcept { return m_event == nullptr; }

        bool await_suspend(std::coroutine_handle<> awaiter) noexcept;

        void await_resume() const noexcept {}

    private:

        friend class async_auto_reset_event;

        const async_auto_reset_event* m_event;
        async_auto_reset_event_operation* m_next;
        std::coroutine_handle<> m_awaiter;
        std::atomic<std::uint32_t> m_refCount;

    };
}
#endif //POCORO_ASYNC_AUTO_RESET_EVENT_HPP
