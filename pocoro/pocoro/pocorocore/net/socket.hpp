//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_SOCKET_HPP
#define POCORO_SOCKET_HPP

#include "../config.hpp"

#include "ip_endpoint.hpp"
#include "socket_accept_operation.hpp"
#include "socket_connect_operation.hpp"
#include "socket_disconnect_operation.hpp"
#include "socket_recv_operation.hpp"
#include "socket_recv_from_operation.hpp"
#include "socket_send_operation.hpp"
#include "socket_send_to_operation.hpp"

#include "../cancellation_token.hpp"

#if POCORO_OS_WINNT
# include "../detail/win32.hpp"
#endif

namespace pocoro
{
    class io_service;

    namespace net
    {
        class socket
        {
        public:

            /// Create a socket that can be used to communicate using TCP/IPv4 protocol.
            ///
            /// \param ioSvc
            /// The I/O service the socket will use for dispatching I/O completion events.
            ///
            /// \return
            /// The newly created socket.
            ///
            /// \throws std::system_error
            /// If the socket could not be created for some reason.
            static socket create_tcpv4(io_service& ioSvc);

            /// Create a socket that can be used to communicate using TCP/IPv6 protocol.
            ///
            /// \param ioSvc
            /// The I/O service the socket will use for dispatching I/O completion events.
            ///
            /// \return
            /// The newly created socket.
            ///
            /// \throws std::system_error
            /// If the socket could not be created for some reason.
            static socket create_tcpv6(io_service& ioSvc);

            /// Create a socket that can be used to communicate using UDP/IPv4 protocol.
            ///
            /// \param ioSvc
            /// The I/O service the socket will use for dispatching I/O completion events.
            ///
            /// \return
            /// The newly created socket.
            ///
            /// \throws std::system_error
            /// If the socket could not be created for some reason.
            static socket create_udpv4(io_service& ioSvc);

            /// Create a socket that can be used to communicate using UDP/IPv6 protocol.
            ///
            /// \param ioSvc
            /// The I/O service the socket will use for dispatching I/O completion events.
            ///
            /// \return
            /// The newly created socket.
            ///
            /// \throws std::system_error
            /// If the socket could not be created for some reason.
            static socket create_udpv6(io_service& ioSvc);

            socket(socket&& other) noexcept;

            /// Closes the socket, releasing any associated resources.
            ///
            /// If the socket still has an open connection then the connection will be
            /// reset. The destructor will not block waiting for queueud data to be sent.
            /// If you need to ensure that queued data is delivered then you must call
            /// disconnect() and wait until the disconnect operation completes.
            ~socket();

            socket& operator=(socket&& other) noexcept;

#if POCORO_OS_WINNT
            /// Get the Win32 socket handle assocaited with this socket.
			pocoro::detail::win32::socket_t native_handle() noexcept { return m_handle; }

			/// Query whether I/O operations that complete synchronously will skip posting
			/// an I/O completion event to the I/O completion port.
			///
			/// The operation class implementations can use this to determine whether or not
			/// it should immediately resume the coroutine on the current thread upon an
			/// operation completing synchronously or whether it should suspend the coroutine
			/// and wait until the I/O completion event is dispatched to an I/O thread.
			bool skip_completion_on_success() noexcept { return m_skipCompletionOnSuccess; }
#endif

            /// Get the address and port of the local end-point.
            ///
            /// If the socket is not bound then this will be the unspecified end-point
            /// of the socket's associated address-family.
            const ip_endpoint& local_endpoint() const noexcept { return m_localEndPoint; }
            
            const ip_endpoint& remote_endpoint() const noexcept { return m_remoteEndPoint; }

            void bind(const ip_endpoint& localEndPoint);

            void listen();

            void listen(std::uint32_t backlog);


            [[nodiscard]]
            socket_connect_operation connect(const ip_endpoint& remoteEndPoint) noexcept;


            [[nodiscard]]
            socket_connect_operation_cancellable connect(
                    const ip_endpoint& remoteEndPoint,
                    cancellation_token ct) noexcept;

            [[nodiscard]]
            socket_accept_operation accept(socket& acceptingSocket) noexcept;
            [[nodiscard]]
            socket_accept_operation_cancellable accept(
                    socket& acceptingSocket,
                    cancellation_token ct) noexcept;

            [[nodiscard]]
            socket_disconnect_operation disconnect() noexcept;
            [[nodiscard]]
            socket_disconnect_operation_cancellable disconnect(cancellation_token ct) noexcept;

            [[nodiscard]]
            socket_send_operation send(
                    const void* buffer,
                    std::size_t size) noexcept;
            [[nodiscard]]
            socket_send_operation_cancellable send(
                    const void* buffer,
                    std::size_t size,
                    cancellation_token ct) noexcept;

            [[nodiscard]]
            socket_recv_operation recv(
                    void* buffer,
                    std::size_t size) noexcept;
            [[nodiscard]]
            socket_recv_operation_cancellable recv(
                    void* buffer,
                    std::size_t size,
                    cancellation_token ct) noexcept;

            [[nodiscard]]
            socket_recv_from_operation recv_from(
                    void* buffer,
                    std::size_t size) noexcept;
            [[nodiscard]]
            socket_recv_from_operation_cancellable recv_from(
                    void* buffer,
                    std::size_t size,
                    cancellation_token ct) noexcept;

            [[nodiscard]]
            socket_send_to_operation send_to(
                    const ip_endpoint& destination,
                    const void* buffer,
                    std::size_t size) noexcept;
            [[nodiscard]]
            socket_send_to_operation_cancellable send_to(
                    const ip_endpoint& destination,
                    const void* buffer,
                    std::size_t size,
                    cancellation_token ct) noexcept;

            void close_send();
            void close_recv();

        private:

            friend class socket_accept_operation_impl;
            friend class socket_connect_operation_impl;

#if POCORO_OS_WINNT
            explicit socket(
				pocoro::detail::win32::socket_t handle,
				bool skipCompletionOnSuccess) noexcept;
#endif

#if POCORO_OS_WINNT
            pocoro::detail::win32::socket_t m_handle;
			bool m_skipCompletionOnSuccess;
#endif

            ip_endpoint m_localEndPoint;
            ip_endpoint m_remoteEndPoint;

        };
    }
}
#endif //POCORO_SOCKET_HPP
