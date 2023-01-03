
#include "../pocorocore/detail/win32.hpp"

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

void pocoro::detail::win32::safe_handle::close() noexcept
{
	if (m_handle != nullptr && m_handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_handle);
		m_handle = nullptr;
	}
}
