#ifndef POCORO_PRIVATE_SOCKET_HELPERS_HPP_INCLUDED
#define POCORO_PRIVATE_SOCKET_HELPERS_HPP_INCLUDED

#include "../pocorocore/config.hpp"

#if POCORO_OS_WINNT
# include "../pocorocore/detail/win32.hpp"
struct sockaddr;
struct sockaddr_storage;
#endif

namespace pocoro
{
	namespace net
	{
		class ip_endpoint;

		namespace detail
		{
#if POCORO_OS_WINNT
			/// Convert a sockaddr to an IP endpoint.
			ip_endpoint sockaddr_to_ip_endpoint(const sockaddr& address) noexcept;

			/// Converts an ip_endpoint to a sockaddr structure.
			///
			/// \param endPoint
			/// The IP endpoint to convert to a sockaddr structure.
			///
			/// \param address
			/// The sockaddr structure to populate.
			///
			/// \return
			/// The length of the sockaddr structure that was populated.
			int ip_endpoint_to_sockaddr(
				const ip_endpoint& endPoint,
				std::reference_wrapper<sockaddr_storage> address) noexcept;

#endif
		}
	}
}

#endif
