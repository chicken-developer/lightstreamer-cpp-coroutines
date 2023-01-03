//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_SOCKET_ACCEPT_OPERATION_HPP
#define POCORO_SOCKET_ACCEPT_OPERATION_HPP

#include "../config.hpp"
#include "../cancellation_token.hpp"
#include "../cancellation_registration.hpp"

#if POCORO_OS_WINNT
# include "../detail/win32.hpp"
# include "../detail/win32_overlapped_operation.hpp"

# include <atomic>
# include <optional>
# include <coroutine>

namespace pocoro
{
	namespace net
	{
		class socket;

		class socket_accept_operation_impl
		{
		public:

			socket_accept_operation_impl(
				socket& listeningSocket,
				socket& acceptingSocket) noexcept
				: m_listeningSocket(listeningSocket)
				, m_acceptingSocket(acceptingSocket)
			{}

			bool try_start(pocoro::detail::win32_overlapped_operation_base& operation) noexcept;
			void cancel(pocoro::detail::win32_overlapped_operation_base& operation) noexcept;
			void get_result(pocoro::detail::win32_overlapped_operation_base& operation);

		private:

#if POCORO_COMPILER_MSVC
# pragma warning(push)
# pragma warning(disable : 4324) // Structure padded due to alignment
#endif

			socket& m_listeningSocket;
			socket& m_acceptingSocket;
			alignas(8) std::uint8_t m_addressBuffer[88];

#if POCORO_COMPILER_MSVC
# pragma warning(pop)
#endif

		};

		class socket_accept_operation
			: public pocoro::detail::win32_overlapped_operation<socket_accept_operation>
		{
		public:

			socket_accept_operation(
				socket& listeningSocket,
				socket& acceptingSocket) noexcept
				: m_impl(listeningSocket, acceptingSocket)
			{}

		private:

			friend class pocoro::detail::win32_overlapped_operation<socket_accept_operation>;

			bool try_start() noexcept { return m_impl.try_start(*this); }
			void get_result() { m_impl.get_result(*this); }

			socket_accept_operation_impl m_impl;

		};

		class socket_accept_operation_cancellable
			: public pocoro::detail::win32_overlapped_operation_cancellable<socket_accept_operation_cancellable>
		{
		public:

			socket_accept_operation_cancellable(
				socket& listeningSocket,
				socket& acceptingSocket,
				cancellation_token&& ct) noexcept
				: pocoro::detail::win32_overlapped_operation_cancellable<socket_accept_operation_cancellable>(std::move(ct))
				, m_impl(listeningSocket, acceptingSocket)
			{}

		private:

			friend class pocoro::detail::win32_overlapped_operation_cancellable<socket_accept_operation_cancellable>;

			bool try_start() noexcept { return m_impl.try_start(*this); }
			void cancel() noexcept { m_impl.cancel(*this); }
			void get_result() { m_impl.get_result(*this); }

			socket_accept_operation_impl m_impl;

		};
	}
}

#endif // POCORO_OS_WINNT

#endif //POCORO_SOCKET_ACCEPT_OPERATION_HPP
