//
// Created by quynh on 1/4/2023.
//

#ifndef POCORO_FILE_HPP
#define POCORO_FILE_HPP

#include "config.hpp"

#include "file_open_mode.hpp"
#include "file_share_mode.hpp"
#include "file_buffering_mode.hpp"

#if POCORO_OS_WINNT
# include "detail/win32.hpp"
#endif

#include <filesystem>

namespace pocoro
{
    class io_service;

    class file
    {
    public:

        file(file&& other) noexcept = default;

        virtual ~file();

        /// Get the size of the file in bytes.
        std::uint64_t size() const;

    protected:

#if POCORO_OS_WINNT
    file(pocoro::detail::win32::safe_handle&& fileHandle) noexcept;

		static pocoro::detail::win32::safe_handle open(
                pocoro::detail::win32::dword_t fileAccess,
			io_service& ioService,
			const std::filesystem::path& path,
			pocoro::file_open_mode openMode,
            pocoro::file_share_mode shareMode,
            pocoro::file_buffering_mode bufferingMode);

        pocoro::detail::win32::safe_handle m_fileHandle;
#endif

    };
}

#endif //POCORO_FILE_HPP
