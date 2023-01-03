//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_ASYNC_MUTEX_HPP
#define POCORO_ASYNC_MUTEX_HPP

#include <coroutine>
#include <atomic>
#include <cstdint>
#include <mutex> // for std::adopt_lock_t

namespace pocoro
{
    class async_mutex_lock;
    class async_mutex_lock_operation;
    class async_mutex_scoped_lock_operation;

    class async_mutex
    {
    public:

        async_mutex() noexcept;

        ~async_mutex();

        bool try_lock() noexcept;

        async_mutex_lock_operation lock_async() noexcept;

        async_mutex_scoped_lock_operation scoped_lock_async() noexcept;

        void unlock();

    private:

        friend class async_mutex_lock_operation;

        static constexpr std::uintptr_t not_locked = 1;

        // assume == reinterpret_cast<std::uintptr_t>(static_cast<void*>(nullptr))
        static constexpr std::uintptr_t locked_no_waiters = 0;

        std::atomic<std::uintptr_t> m_state;

        async_mutex_lock_operation* m_waiters;

    };

    class async_mutex_lock
    {
    public:

        explicit async_mutex_lock(async_mutex& mutex, std::adopt_lock_t) noexcept
                : m_mutex(&mutex)
        {}

        async_mutex_lock(async_mutex_lock&& other) noexcept
                : m_mutex(other.m_mutex)
        {
            other.m_mutex = nullptr;
        }

        async_mutex_lock(const async_mutex_lock& other) = delete;
        async_mutex_lock& operator=(const async_mutex_lock& other) = delete;

        // Releases the lock.
        ~async_mutex_lock()
        {
            if (m_mutex != nullptr)
            {
                m_mutex->unlock();
            }
        }

    private:

        async_mutex* m_mutex;

    };

    class async_mutex_lock_operation
    {
    public:

        explicit async_mutex_lock_operation(async_mutex& mutex) noexcept
                : m_mutex(mutex)
        {}

        bool await_ready() const noexcept { return false; }
        bool await_suspend(std::coroutine_handle<> awaiter) noexcept;
        void await_resume() const noexcept {}

    protected:

        friend class async_mutex;

        async_mutex& m_mutex;

    private:

        async_mutex_lock_operation* m_next;
        std::coroutine_handle<> m_awaiter;

    };

    class async_mutex_scoped_lock_operation : public async_mutex_lock_operation
    {
    public:

        using async_mutex_lock_operation::async_mutex_lock_operation;

        [[nodiscard]]
        async_mutex_lock await_resume() const noexcept
        {
            return async_mutex_lock{ m_mutex, std::adopt_lock };
        }

    };
}

#endif //POCORO_ASYNC_MUTEX_HPP
