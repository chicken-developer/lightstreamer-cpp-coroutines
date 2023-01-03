//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_LIGHTWEIGHT_MANUAL_RESET_EVENT_HPP
#define POCORO_LIGHTWEIGHT_MANUAL_RESET_EVENT_HPP

#include <mutex>
#include <condition_variable>

namespace pocoro::detail
    {
        class lightweight_manual_reset_event
        {
        public:

            lightweight_manual_reset_event(bool initiallySet = false);

            ~lightweight_manual_reset_event();

            void set() noexcept;

            void reset() noexcept;

            void wait() noexcept;

        private:
            std::mutex m_mutex;
            std::condition_variable m_cv;
            bool m_isSet;

        };
}

#endif //POCORO_LIGHTWEIGHT_MANUAL_RESET_EVENT_HPP
