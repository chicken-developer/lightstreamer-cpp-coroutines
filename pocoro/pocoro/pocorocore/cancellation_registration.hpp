//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_CANCELLATION_REGISTRATION_HPP
#define POCORO_CANCELLATION_REGISTRATION_HPP


#include "cancellation_token.hpp"

#include <functional>
#include <utility>
#include <type_traits>
#include <atomic>
#include <cstdint>

namespace pocoro
{
    namespace detail
    {
        class cancellation_state;
        struct cancellation_registration_list_chunk;
        struct cancellation_registration_state;
    }

    class cancellation_registration
    {
    public:


        template<
                typename FUNC,
                typename = std::enable_if_t<std::is_constructible_v<std::function<void()>, FUNC&&>>>
        cancellation_registration(cancellation_token token, FUNC&& callback)
                : m_callback(std::forward<FUNC>(callback))
        {
            register_callback(std::move(token));
        }

        cancellation_registration(const cancellation_registration& other) = delete;
        cancellation_registration& operator=(const cancellation_registration& other) = delete;

        ~cancellation_registration();

    private:

        friend class detail::cancellation_state;
        friend struct detail::cancellation_registration_state;

        void register_callback(cancellation_token&& token);

        detail::cancellation_state* m_state;
        std::function<void()> m_callback;
        detail::cancellation_registration_list_chunk* m_chunk;
        std::uint32_t m_entryIndex;
    };
}

#endif //POCORO_CANCELLATION_REGISTRATION_HPP
