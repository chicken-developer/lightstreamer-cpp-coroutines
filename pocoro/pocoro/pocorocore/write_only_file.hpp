//
// Created by quynh on 1/4/2023.
//

#ifndef POCORO_WRITE_ONLY_FILE_HPP
#define POCORO_WRITE_ONLY_FILE_HPP

#include "writable_file.hpp"
#include "file_share_mode.hpp"
#include "file_buffering_mode.hpp"
#include "file_open_mode.hpp"

#include <filesystem>

namespace pocoro
{
	class write_only_file : public writable_file
	{
	public:

		/// Open a file for write-only access.
		///
		/// \param ioContext
		/// The I/O context to use when dispatching I/O completion events.
		/// When asynchronous write operations on this file complete the
		/// completion events will be dispatched to an I/O thread associated
		/// with the I/O context.
		///
		/// \param pathMode
		/// Path of the file to open.
		///
		/// \param openMode
		/// Specifies how the file should be opened and how to handle cases
		/// when the file exists or doesn't exist.
		///
		/// \param shareMode
		/// Specifies the access to be allowed on the file concurrently with this file access.
		///
		/// \param bufferingMode
		/// Specifies the modes/hints to provide to the OS that affects the behaviour
		/// of its file buffering.
		///
		/// \return
		/// An object that can be used to write to the file.
		///
		/// \throw std::system_error
		/// If the file could not be opened for write.
		[[nodiscard]]
		static write_only_file open(
			io_service& ioService,
			const std::filesystem::path& path,
			file_open_mode openMode = file_open_mode::create_or_open,
			file_share_mode shareMode = file_share_mode::none,
			file_buffering_mode bufferingMode = file_buffering_mode::default_);

	protected:

#if POCORO_OS_WINNT
		write_only_file(detail::win32::safe_handle&& fileHandle) noexcept;
#endif

	};
}

#endif //POCORO_WRITE_ONLY_FILE_HPP
