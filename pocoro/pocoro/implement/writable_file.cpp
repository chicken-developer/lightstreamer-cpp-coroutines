
#include "../pocorocore/writable_file.hpp"

#include <system_error>

#if POCORO_OS_WINNT
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <Windows.h>

void pocoro::writable_file::set_size(
	std::uint64_t fileSize)
{
	LARGE_INTEGER position;
	position.QuadPart = fileSize;

	BOOL ok = ::SetFilePointerEx(m_fileHandle.handle(), position, nullptr, FILE_BEGIN);
	if (!ok)
	{
		DWORD errorCode = ::GetLastError();
		throw std::system_error
		{
			static_cast<int>(errorCode),
			std::system_category(),
			"error setting file size: SetFilePointerEx"
		};
	}

	ok = ::SetEndOfFile(m_fileHandle.handle());
	if (!ok)
	{
		DWORD errorCode = ::GetLastError();
		throw std::system_error
		{
			static_cast<int>(errorCode),
			std::system_category(),
			"error setting file size: SetEndOfFile"
		};
	}
}

pocoro::file_write_operation pocoro::writable_file::write(
	std::uint64_t offset,
	const void* buffer,
	std::size_t byteCount) noexcept
{
	return file_write_operation{
		m_fileHandle.handle(),
		offset,
		buffer,
		byteCount
	};
}

pocoro::file_write_operation_cancellable pocoro::writable_file::write(
	std::uint64_t offset,
	const void* buffer,
	std::size_t byteCount,
	cancellation_token ct) noexcept
{
	return file_write_operation_cancellable{
		m_fileHandle.handle(),
		offset,
		buffer,
		byteCount,
		std::move(ct)
	};
}

#endif
