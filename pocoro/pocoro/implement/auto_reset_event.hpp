#ifndef POCORO_AUTO_RESET_EVENT_HPP_INCLUDED
#define POCORO_AUTO_RESET_EVENT_HPP_INCLUDED

#include "../pocorocore/config.hpp"

#if POCORO_OS_WINNT
# include "../pocorocore/detail/win32.hpp"
#else
# include <mutex>
# include <condition_variable>
#endif

namespace pocoro
{
	class auto_reset_event
	{
	public:

		auto_reset_event(bool initiallySet = false);

		~auto_reset_event();

		void set();

		void wait();

	private:

#if POCORO_OS_WINNT
		pocoro::detail::win32::safe_handle m_event;
#else
		std::mutex m_mutex;
		std::condition_variable m_cv;
		bool m_isSet;
#endif

	};
}

#endif
