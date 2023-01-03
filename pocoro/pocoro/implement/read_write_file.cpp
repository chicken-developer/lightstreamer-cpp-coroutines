
#include "../pocorocore/read_write_file.hpp"

#if POCORO_OS_WINNT
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <Windows.h>

pocoro::read_write_file pocoro::read_write_file::open(
	io_service& ioService,
	const std::filesystem::path& path,
	file_open_mode openMode,
	file_share_mode shareMode,
	file_buffering_mode bufferingMode)
{
	return read_write_file(file::open(
		GENERIC_READ | GENERIC_WRITE,
		ioService,
		path,
		openMode,
		shareMode,
		bufferingMode));
}

pocoro::read_write_file::read_write_file(
	detail::win32::safe_handle&& fileHandle) noexcept
	: file(std::move(fileHandle))
	, readable_file(detail::win32::safe_handle{})
	, writable_file(detail::win32::safe_handle{})
{
}

#endif
