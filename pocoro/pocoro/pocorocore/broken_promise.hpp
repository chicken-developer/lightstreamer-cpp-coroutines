//
// Created by quynh on 1/4/2023.
//

#ifndef POCORO_BROKEN_PROMISE_HPP
#define POCORO_BROKEN_PROMISE_HPP
#include <stdexcept>

namespace pocoro
{
    /// \brief
    /// Exception thrown when you attempt to retrieve the result of
    /// a task that has been detached from its promise/coroutine.
    class broken_promise : public std::logic_error
    {
    public:
        broken_promise()
                : std::logic_error("broken promise")
        {}
    };
}

#endif //POCORO_BROKEN_PROMISE_HPP
