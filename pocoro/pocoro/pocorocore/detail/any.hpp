//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_ANY_HPP
#define POCORO_ANY_HPP

namespace pocoro::detail
{
        // Helper type that can be cast-to from any type.
        struct any
        {
            template<typename T>
            any(T&&) noexcept
            {}
        };

}
#endif //POCORO_ANY_HPP
