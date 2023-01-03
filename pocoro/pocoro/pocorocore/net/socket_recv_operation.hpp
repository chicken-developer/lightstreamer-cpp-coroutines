//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_SOCKET_RECV_OPERATION_HPP
#define POCORO_SOCKET_RECV_OPERATION_HPP

#include "../config.hpp"
#include "../cancellation_token.hpp"

#include <cstdint>

#if POCORO_OS_WINNT
# include "../detail/win32.hpp"
# include "../detail/win32_overlapped_operation.hpp"

namespace pocoro::net
{
	class socket;

	class socket_recv_operation_impl
	{
	public:

		socket_recv_operation_impl(
			socket& s,
			void* buffer,
			std::size_t byteCount) noexcept
			: m_socket(s)
			, m_buffer(buffer, byteCount)
		{}

		bool try_start(pocoro::detail::win32_overlapped_operation_base& operation) noexcept;
		void cancel(pocoro::detail::win32_overlapped_operation_base& operation) noexcept;

	private:

		socket& m_socket;
		pocoro::detail::win32::wsabuf m_buffer;

	};

	class socket_recv_operation
		: public pocoro::detail::win32_overlapped_operation<socket_recv_operation>
	{
	public:

		socket_recv_operation(
			socket& s,
			void* buffer,
			std::size_t byteCount) noexcept
			: m_impl(s, buffer, byteCount)
		{}

	private:

		friend class pocoro::detail::win32_overlapped_operation<socket_recv_operation>;

		bool try_start() noexcept { return m_impl.try_start(*this); }

		socket_recv_operation_impl m_impl;

	};

	class socket_recv_operation_cancellable
		: public pocoro::detail::win32_overlapped_operation_cancellable<socket_recv_operation_cancellable>
	{
	public:

		socket_recv_operation_cancellable(
			socket& s,
			void* buffer,
			std::size_t byteCount,
			cancellation_token&& ct) noexcept
			: pocoro::detail::win32_overlapped_operation_cancellable<socket_recv_operation_cancellable>(std::move(ct))
			, m_impl(s, buffer, byteCount)
		{}

	private:

		friend class pocoro::detail::win32_overlapped_operation_cancellable<socket_recv_operation_cancellable>;

		bool try_start() noexcept { return m_impl.try_start(*this); }
		void cancel() noexcept { m_impl.cancel(*this); }

		socket_recv_operation_impl m_impl;

	};

}

#endif // POCORO_OS_WINNT

#endif //POCORO_SOCKET_RECV_OPERATION_HPP
