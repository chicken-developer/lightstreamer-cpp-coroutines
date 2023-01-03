//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_IPV4_ADDRESS_HPP
#define POCORO_IPV4_ADDRESS_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

namespace pocoro::net
{
    class ipv4_address
    {
        using bytes_t = std::uint8_t[4];

    public:

        constexpr ipv4_address()
                : m_bytes{ 0, 0, 0, 0 }
        {}

        explicit constexpr ipv4_address(std::uint32_t integer)
                : m_bytes{
                static_cast<std::uint8_t>(integer >> 24),
                static_cast<std::uint8_t>(integer >> 16),
                static_cast<std::uint8_t>(integer >> 8),
                static_cast<std::uint8_t>(integer) }
        {}

        explicit constexpr ipv4_address(const std::uint8_t(&bytes)[4])
                : m_bytes{ bytes[0], bytes[1], bytes[2], bytes[3] }
        {}

        explicit constexpr ipv4_address(
                std::uint8_t b0,
                std::uint8_t b1,
                std::uint8_t b2,
                std::uint8_t b3)
                : m_bytes{ b0, b1, b2, b3 }
        {}

        constexpr const bytes_t& bytes() const { return m_bytes; }

        constexpr std::uint32_t to_integer() const
        {
            return
                    std::uint32_t(m_bytes[0]) << 24 |
                    std::uint32_t(m_bytes[1]) << 16 |
                    std::uint32_t(m_bytes[2]) << 8 |
                    std::uint32_t(m_bytes[3]);
        }

        static constexpr ipv4_address loopback()
        {
            return ipv4_address(127, 0, 0, 1);
        }

        constexpr bool is_loopback() const
        {
            return m_bytes[0] == 127;
        }

        constexpr bool is_private_network() const
        {
            return m_bytes[0] == 10 ||
                   (m_bytes[0] == 172 && (m_bytes[1] & 0xF0) == 0x10) ||
                   (m_bytes[0] == 192 && m_bytes[2] == 168);
        }

        constexpr bool operator==(ipv4_address other) const
        {
            return
                    m_bytes[0] == other.m_bytes[0] &&
                    m_bytes[1] == other.m_bytes[1] &&
                    m_bytes[2] == other.m_bytes[2] &&
                    m_bytes[3] == other.m_bytes[3];
        }

        constexpr bool operator!=(ipv4_address other) const
        {
            return !(*this == other);
        }

        constexpr bool operator<(ipv4_address other) const
        {
            return to_integer() < other.to_integer();
        }

        constexpr bool operator>(ipv4_address other) const
        {
            return other < *this;
        }

        constexpr bool operator<=(ipv4_address other) const
        {
            return !(other < *this);
        }

        constexpr bool operator>=(ipv4_address other) const
        {
            return !(*this < other);
        }

        static std::optional<ipv4_address> from_string(std::string_view string) noexcept;

        std::string to_string() const;

    private:

        alignas(std::uint32_t) std::uint8_t m_bytes[4];

    };
}

#endif //POCORO_IPV4_ADDRESS_HPP
