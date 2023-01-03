#ifndef POCORO_SYNC_WAIT_HPP
#define POCORO_SYNC_WAIT_HPP

#include "detail/lightweight_manual_reset_event.hpp"
#include "detail/sync_wait_task.hpp"
#include "awaitable_traits.hpp"

#include <cstdint>
#include <atomic>

namespace pocoro
{
	template<typename AWAITABLE>
	auto sync_wait(AWAITABLE&& awaitable)
		-> typename pocoro::awaitable_traits<AWAITABLE&&>::await_result_t
	{
#if POCORO_COMPILER_MSVC
		// HACK: Need to explicitly specify template argument to make_sync_wait_task
		// here to work around a bug in MSVC when passing parameters by universal
		// reference to a coroutine which causes the compiler to think it needs to
		// 'move' parameters passed by rvalue reference.
		auto task = detail::make_sync_wait_task<AWAITABLE>(awaitable);
#else
		auto task = detail::make_sync_wait_task(std::forward<AWAITABLE>(awaitable));
#endif
		detail::lightweight_manual_reset_event event;
		task.start(event);
		event.wait();
		return task.result();
	}
}

#endif //POCORO_SYNC_WAIT_HPP