
#include "../pocorocore/readable_file.hpp"

#if POCORO_OS_WINNT

pocoro::file_read_operation pocoro::readable_file::read(
	std::uint64_t offset,
	void* buffer,
	std::size_t byteCount) const noexcept
{
	return file_read_operation(
		m_fileHandle.handle(),
		offset,
		buffer,
		byteCount);
}

pocoro::file_read_operation_cancellable pocoro::readable_file::read(
	std::uint64_t offset,
	void* buffer,
	std::size_t byteCount,
	cancellation_token ct) const noexcept
{
	return file_read_operation_cancellable(
		m_fileHandle.handle(),
		offset,
		buffer,
		byteCount,
		std::move(ct));
}

#endif
