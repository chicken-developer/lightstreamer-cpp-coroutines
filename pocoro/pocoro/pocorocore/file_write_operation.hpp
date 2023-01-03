//
// Created by quynh on 1/4/2023.
//

#ifndef POCORO_FILE_WRITE_OPERATION_HPP
#define POCORO_FILE_WRITE_OPERATION_HPP

#include "config.hpp"
#include "cancellation_registration.hpp"
#include "cancellation_token.hpp"

#include <atomic>
#include <optional>
#include <coroutine>

#if POCORO_OS_WINNT
# include "detail/win32.hpp"
# include "detail/win32_overlapped_operation.hpp"

namespace pocoro
{
	class file_write_operation_impl
	{
	public:

		file_write_operation_impl(
			detail::win32::handle_t fileHandle,
			const void* buffer,
			std::size_t byteCount) noexcept
			: m_fileHandle(fileHandle)
			, m_buffer(buffer)
			, m_byteCount(byteCount)
		{}

		bool try_start(pocoro::detail::win32_overlapped_operation_base& operation) noexcept;
		void cancel(pocoro::detail::win32_overlapped_operation_base& operation) noexcept;

	private:

		detail::win32::handle_t m_fileHandle;
		const void* m_buffer;
		std::size_t m_byteCount;

	};

	class file_write_operation
		: public pocoro::detail::win32_overlapped_operation<file_write_operation>
	{
	public:

		file_write_operation(
			detail::win32::handle_t fileHandle,
			std::uint64_t fileOffset,
			const void* buffer,
			std::size_t byteCount) noexcept
			: pocoro::detail::win32_overlapped_operation<file_write_operation>(fileOffset)
			, m_impl(fileHandle, buffer, byteCount)
		{}

	private:

		friend class pocoro::detail::win32_overlapped_operation<file_write_operation>;

		bool try_start() noexcept { return m_impl.try_start(*this); }

		file_write_operation_impl m_impl;

	};

	class file_write_operation_cancellable
		: public pocoro::detail::win32_overlapped_operation_cancellable<file_write_operation_cancellable>
	{
	public:

		file_write_operation_cancellable(
			detail::win32::handle_t fileHandle,
			std::uint64_t fileOffset,
			const void* buffer,
			std::size_t byteCount,
			cancellation_token&& ct) noexcept
			: pocoro::detail::win32_overlapped_operation_cancellable<file_write_operation_cancellable>(fileOffset, std::move(ct))
			, m_impl(fileHandle, buffer, byteCount)
		{}

	private:

		friend class pocoro::detail::win32_overlapped_operation_cancellable<file_write_operation_cancellable>;

		bool try_start() noexcept { return m_impl.try_start(*this); }
		void cancel() noexcept { m_impl.cancel(*this); }

		file_write_operation_impl m_impl;

	};
}

#endif // POCORO_OS_WINNT

#endif //POCORO_FILE_WRITE_OPERATION_HPP
