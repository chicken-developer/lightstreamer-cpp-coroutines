//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_SOCKET_DISCONNECT_OPERATION_HPP
#define POCORO_SOCKET_DISCONNECT_OPERATION_HPP

#include "../config.hpp"
#include "../cancellation_token.hpp"

#if POCORO_OS_WINNT
# include "../detail/win32.hpp"
# include "../detail/win32_overlapped_operation.hpp"

namespace pocoro
{
	namespace net
	{
		class socket;

		class socket_disconnect_operation_impl
		{
		public:

			socket_disconnect_operation_impl(socket& socket) noexcept
				: m_socket(socket)
			{}

			bool try_start(pocoro::detail::win32_overlapped_operation_base& operation) noexcept;
			void cancel(pocoro::detail::win32_overlapped_operation_base& operation) noexcept;
			void get_result(pocoro::detail::win32_overlapped_operation_base& operation);

		private:

			socket& m_socket;

		};

		class socket_disconnect_operation
			: public pocoro::detail::win32_overlapped_operation<socket_disconnect_operation>
		{
		public:

			socket_disconnect_operation(socket& socket) noexcept
				: m_impl(socket)
			{}

		private:

			friend class pocoro::detail::win32_overlapped_operation<socket_disconnect_operation>;

			bool try_start() noexcept { return m_impl.try_start(*this); }
			void get_result() { m_impl.get_result(*this); }

			socket_disconnect_operation_impl m_impl;

		};

		class socket_disconnect_operation_cancellable
			: public pocoro::detail::win32_overlapped_operation_cancellable<socket_disconnect_operation_cancellable>
		{
		public:

			socket_disconnect_operation_cancellable(socket& socket, cancellation_token&& ct) noexcept
				: pocoro::detail::win32_overlapped_operation_cancellable<socket_disconnect_operation_cancellable>(std::move(ct))
				, m_impl(socket)
			{}

		private:

			friend class pocoro::detail::win32_overlapped_operation_cancellable<socket_disconnect_operation_cancellable>;

			bool try_start() noexcept { return m_impl.try_start(*this); }
			void cancel() noexcept { m_impl.cancel(*this); }
			void get_result() { m_impl.get_result(*this); }

			socket_disconnect_operation_impl m_impl;

		};
	}
}

#endif // POCORO_OS_WINNT

#endif //POCORO_SOCKET_DISCONNECT_OPERATION_HPP
