
#include "../pocorocore/net/socket_recv_from_operation.hpp"
#include "../pocorocore/net/socket.hpp"

#if POCORO_OS_WINNT
# include "socket_helpers.hpp"

# include <WinSock2.h>
# include <WS2tcpip.h>
# include <MSWSock.h>
# include <Windows.h>

bool pocoro::net::socket_recv_from_operation_impl::try_start(
	pocoro::detail::win32_overlapped_operation_base& operation) noexcept
{
	static_assert(
		sizeof(m_sourceSockaddrStorage) >= sizeof(SOCKADDR_IN) &&
		sizeof(m_sourceSockaddrStorage) >= sizeof(SOCKADDR_IN6));
	static_assert(
		sockaddrStorageAlignment >= alignof(SOCKADDR_IN) &&
		sockaddrStorageAlignment >= alignof(SOCKADDR_IN6));

	// Need to read this flag before starting the operation, otherwise
	// it may be possible that the operation will complete immediately
	// on another thread, resume the coroutine and then destroy the
	// socket before we get a chance to read it.
	const bool skipCompletionOnSuccess = m_socket.skip_completion_on_success();

	m_sourceSockaddrLength = sizeof(m_sourceSockaddrStorage);

	DWORD numberOfBytesReceived = 0;
	DWORD flags = 0;
	int result = ::WSARecvFrom(
		m_socket.native_handle(),
		reinterpret_cast<WSABUF*>(&m_buffer),
		1, // buffer count
		&numberOfBytesReceived,
		&flags,
		reinterpret_cast<sockaddr*>(&m_sourceSockaddrStorage),
		&m_sourceSockaddrLength,
		operation.get_overlapped(),
		nullptr);
	if (result == SOCKET_ERROR)
	{
		int errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			// Failed synchronously.
			operation.m_errorCode = static_cast<DWORD>(errorCode);
			operation.m_numberOfBytesTransferred = numberOfBytesReceived;
			return false;
		}
	}
	else if (skipCompletionOnSuccess)
	{
		// Completed synchronously, no completion event will be posted to the IOCP.
		operation.m_errorCode = ERROR_SUCCESS;
		operation.m_numberOfBytesTransferred = numberOfBytesReceived;
		return false;
	}

	// Operation will complete asynchronously.
	return true;
}

void pocoro::net::socket_recv_from_operation_impl::cancel(
	pocoro::detail::win32_overlapped_operation_base& operation) noexcept
{
	(void)::CancelIoEx(
		reinterpret_cast<HANDLE>(m_socket.native_handle()),
		operation.get_overlapped());
}

std::tuple<std::size_t, pocoro::net::ip_endpoint>
pocoro::net::socket_recv_from_operation_impl::get_result(
	pocoro::detail::win32_overlapped_operation_base& operation)
{
	if (operation.m_errorCode != ERROR_SUCCESS)
	{
		throw std::system_error(
			static_cast<int>(operation.m_errorCode),
			std::system_category(),
			"Error receiving message on socket: WSARecvFrom");
	}

	return std::make_tuple(
		static_cast<std::size_t>(operation.m_numberOfBytesTransferred),
		detail::sockaddr_to_ip_endpoint(
			*reinterpret_cast<SOCKADDR*>(&m_sourceSockaddrStorage)));
}

#endif
