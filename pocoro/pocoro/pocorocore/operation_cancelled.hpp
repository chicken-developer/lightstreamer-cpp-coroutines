//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_OPERATION_CANCELLED_HPP
#define POCORO_OPERATION_CANCELLED_HPP

#include <exception>

namespace pocoro
{
    class operation_cancelled : public std::exception
    {
    public:

        operation_cancelled() noexcept
                : std::exception()
        {}

        const char* what() const noexcept override { return "operation cancelled"; }
    };
}

#endif //POCORO_OPERATION_CANCELLED_HPP
