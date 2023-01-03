//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_REMOVE_RVALUE_REFERENCE_HPP
#define POCORO_REMOVE_RVALUE_REFERENCE_HPP

namespace pocoro::detail
{
        template<typename T>
        struct remove_rvalue_reference
        {
            using type = T;
        };

        template<typename T>
        struct remove_rvalue_reference<T&&>
        {
            using type = T;
        };

        template<typename T>
        using remove_rvalue_reference_t = typename remove_rvalue_reference<T>::type;

}
#endif //POCORO_REMOVE_RVALUE_REFERENCE_HPP
