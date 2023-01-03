//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_SOCKET_SEND_TO_OPERATION_HPP
#define POCORO_SOCKET_SEND_TO_OPERATION_HPP

#include "../config.hpp"
#include "../cancellation_token.hpp"
#include "../net/ip_endpoint.hpp"

#include <cstdint>

#if POCORO_OS_WINNT
# include "../detail/win32.hpp"
# include "../detail/win32_overlapped_operation.hpp"

namespace pocoro::net
{
	class socket;

	class socket_send_to_operation_impl
	{
	public:

		socket_send_to_operation_impl(
			socket& s,
			const ip_endpoint& destination,
			const void* buffer,
			std::size_t byteCount) noexcept
			: m_socket(s)
			, m_destination(destination)
			, m_buffer(const_cast<void*>(buffer), byteCount)
		{}

		bool try_start(pocoro::detail::win32_overlapped_operation_base& operation) noexcept;
		void cancel(pocoro::detail::win32_overlapped_operation_base& operation) noexcept;

	private:

		socket& m_socket;
		ip_endpoint m_destination;
		pocoro::detail::win32::wsabuf m_buffer;

	};

	class socket_send_to_operation
		: public pocoro::detail::win32_overlapped_operation<socket_send_to_operation>
	{
	public:

		socket_send_to_operation(
			socket& s,
			const ip_endpoint& destination,
			const void* buffer,
			std::size_t byteCount) noexcept
			: m_impl(s, destination, buffer, byteCount)
		{}

	private:

		friend class pocoro::detail::win32_overlapped_operation<socket_send_to_operation>;

		bool try_start() noexcept { return m_impl.try_start(*this); }

		socket_send_to_operation_impl m_impl;

	};

	class socket_send_to_operation_cancellable
		: public pocoro::detail::win32_overlapped_operation_cancellable<socket_send_to_operation_cancellable>
	{
	public:

		socket_send_to_operation_cancellable(
			socket& s,
			const ip_endpoint& destination,
			const void* buffer,
			std::size_t byteCount,
			cancellation_token&& ct) noexcept
			: pocoro::detail::win32_overlapped_operation_cancellable<socket_send_to_operation_cancellable>(std::move(ct))
			, m_impl(s, destination, buffer, byteCount)
		{}

	private:

		friend class pocoro::detail::win32_overlapped_operation_cancellable<socket_send_to_operation_cancellable>;

		bool try_start() noexcept { return m_impl.try_start(*this); }
		void cancel() noexcept { return m_impl.cancel(*this); }

		socket_send_to_operation_impl m_impl;

	};

}

#endif // POCORO_OS_WINNT

#endif //POCORO_SOCKET_SEND_TO_OPERATION_HPP
