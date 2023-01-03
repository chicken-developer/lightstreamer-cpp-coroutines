
#include "../pocorocore/write_only_file.hpp"

#if POCORO_OS_WINNT
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <Windows.h>

pocoro::write_only_file pocoro::write_only_file::open(
	io_service& ioService,
	const std::filesystem::path& path,
	file_open_mode openMode,
	file_share_mode shareMode,
	file_buffering_mode bufferingMode)
{
	return write_only_file(file::open(
		GENERIC_WRITE,
		ioService,
		path,
		openMode,
		shareMode,
		bufferingMode));
}

pocoro::write_only_file::write_only_file(
	detail::win32::safe_handle&& fileHandle) noexcept
	: file(std::move(fileHandle))
	, writable_file(detail::win32::safe_handle{})
{
}

#endif
