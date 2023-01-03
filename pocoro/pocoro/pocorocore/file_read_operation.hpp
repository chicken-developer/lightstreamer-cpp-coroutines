//
// Created by quynh on 1/4/2023.
//

#ifndef POCORO_FILE_READ_OPERATION_HPP
#define POCORO_FILE_READ_OPERATION_HPP

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
	class file_read_operation_impl
	{
	public:

		file_read_operation_impl(
			pocoro::detail::win32::handle_t fileHandle,
			void* buffer,
			std::size_t byteCount) noexcept
			: m_fileHandle(fileHandle)
			, m_buffer(buffer)
			, m_byteCount(byteCount)
		{}

		bool try_start(pocoro::detail::win32_overlapped_operation_base& operation) noexcept;
		void cancel(pocoro::detail::win32_overlapped_operation_base& operation) noexcept;

	private:

		detail::win32::handle_t m_fileHandle;
		void* m_buffer;
		std::size_t m_byteCount;

	};

	class file_read_operation
		: public pocoro::detail::win32_overlapped_operation<file_read_operation>
	{
	public:

		file_read_operation(
			detail::win32::handle_t fileHandle,
			std::uint64_t fileOffset,
			void* buffer,
			std::size_t byteCount) noexcept
			: pocoro::detail::win32_overlapped_operation<file_read_operation>(fileOffset)
			, m_impl(fileHandle, buffer, byteCount)
		{}

	private:

		friend class pocoro::detail::win32_overlapped_operation<file_read_operation>;

		bool try_start() noexcept { return m_impl.try_start(*this); }

		file_read_operation_impl m_impl;

	};

	class file_read_operation_cancellable
		: public pocoro::detail::win32_overlapped_operation_cancellable<file_read_operation_cancellable>
	{
	public:

		file_read_operation_cancellable(
			detail::win32::handle_t fileHandle,
			std::uint64_t fileOffset,
			void* buffer,
			std::size_t byteCount,
			cancellation_token&& cancellationToken) noexcept
			: pocoro::detail::win32_overlapped_operation_cancellable<file_read_operation_cancellable>(
				fileOffset, std::move(cancellationToken))
			, m_impl(fileHandle, buffer, byteCount)
		{}

	private:

		friend class pocoro::detail::win32_overlapped_operation_cancellable<file_read_operation_cancellable>;

		bool try_start() noexcept { return m_impl.try_start(*this); }
		void cancel() noexcept { m_impl.cancel(*this); }

		file_read_operation_impl m_impl;

	};

#endif
}

#endif //POCORO_FILE_READ_OPERATION_HPP
