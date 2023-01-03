
#include "../pocorocore/read_only_file.hpp"

#if POCORO_OS_WINNT
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <Windows.h>

pocoro::read_only_file pocoro::read_only_file::open(
	io_service& ioService,
	const std::filesystem::path& path,
	file_share_mode shareMode,
	file_buffering_mode bufferingMode)
{
	return read_only_file(file::open(
		GENERIC_READ,
		ioService,
		path,
		file_open_mode::open_existing,
		shareMode,
		bufferingMode));
}

pocoro::read_only_file::read_only_file(
	detail::win32::safe_handle&& fileHandle) noexcept
	: file(std::move(fileHandle))
	, readable_file(detail::win32::safe_handle{})
{
}

#endif
