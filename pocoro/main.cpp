#include <memory>
#include <iostream>
#include "pocoro/pocorocore/net/socket.hpp"
#include "pocoro/pocorocore/io_service.hpp"
#include "pocoro/pocorocore/cancellation_source.hpp"
#include "pocoro/pocorocore/async_scope.hpp"
#include "pocoro/pocorocore/on_scope_exit.hpp"
#include "pocoro/pocorocore/task.hpp"
#include "pocoro/pocorocore/sync_wait.hpp"
#include "pocoro/pocorocore/when_all.hpp"


pocoro::task<void> handle_connection(pocoro::net::socket s)
{
    try
    {
        const size_t bufferSize = 16384;
        auto buffer = std::make_unique<unsigned char[]>(bufferSize);
        size_t bytesRead;
        do {
            // Read some bytes
            bytesRead = co_await s.recv(buffer.get(), bufferSize);

            // Write some bytes
            size_t bytesWritten = 0;
            while (bytesWritten < bytesRead) {
                bytesWritten += co_await s.send(
                        buffer.get() + bytesWritten,
                        bytesRead - bytesWritten);
            }
        } while (bytesRead != 0);

        s.close_send();

        co_await s.disconnect();
    }
    catch (...)
    {
        std::cout << "connection failed" << std::endl;
    }
}

pocoro::task<void> echo_server(
        pocoro::net::ipv4_endpoint endpoint,
        pocoro::io_service& ioSvc,
        pocoro::cancellation_token ct)
{
    pocoro::async_scope scope;

    std::exception_ptr ex;
    try
    {
        auto listeningSocket = pocoro::net::socket::create_tcpv4(ioSvc);
        listeningSocket.bind(endpoint);
        listeningSocket.listen();

        while (true) {
            auto connection = pocoro::net::socket::create_tcpv4(ioSvc);
            co_await listeningSocket.accept(connection, ct);
            scope.spawn(handle_connection(std::move(connection)));
        }
    }
    catch (pocoro::operation_cancelled)
    {
    }
    catch (...)
    {
        ex = std::current_exception();
    }

    // Wait until all handle_connection tasks have finished.
    co_await scope.join();

    if (ex) std::rethrow_exception(ex);
}

int main(int argc, const char* argv[])
{
    pocoro::io_service ioSvc;

    if (argc != 2) return -1;

    auto endpoint = pocoro::net::ipv4_endpoint::from_string("localhost");
    if (!endpoint) return -1;

    (void)pocoro::sync_wait(pocoro::when_all(
            [&]() -> pocoro::task<>
            {
                // Shutdown the event loop once finished.
                auto stopOnExit = pocoro::on_scope_exit([&] { ioSvc.stop(); });

                pocoro::cancellation_source canceller;
                co_await pocoro::when_all(
                        [&]() -> pocoro::task<>
                        {
                            // Run for 30s then stop accepting new connections.
                            co_await ioSvc.schedule_after(std::chrono::seconds(30));
                            canceller.request_cancellation();
                        }(),
                        echo_server(*endpoint, ioSvc, canceller.token()));
            }(),
            [&]() -> pocoro::task<>
            {
                ioSvc.process_events();
                return {};
            }()));

    return 0;
}