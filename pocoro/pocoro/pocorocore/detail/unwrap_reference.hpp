//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_UNWRAP_REFERENCE_HPP
#define POCORO_UNWRAP_REFERENCE_HPP
#include <functional>

namespace pocoro::detail
{
    template<typename T>
    struct unwrap_reference
    {
        using type = T;
    };

    template<typename T>
    struct unwrap_reference<std::reference_wrapper<T>>
    {
        using type = T;
    };

    template<typename T>
    using unwrap_reference_t = typename unwrap_reference<T>::type;
}

#endif //POCORO_UNWRAP_REFERENCE_HPP
