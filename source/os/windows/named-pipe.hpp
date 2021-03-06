/* Copyright(C) 2018 Michael Fabian Dirks <info@xaymar.com>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef OS_WINDOWS_NAMED_PIPE_HPP
#define OS_WINDOWS_NAMED_PIPE_HPP

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <inttypes.h>
#include <memory>
#include <string>
#include <windows.h>
#include "../error.hpp"
#include "../tags.hpp"
#include "async_request.hpp"

namespace os {
	namespace windows {
		enum class pipe_type : int8_t {
			Byte = 0x00,
			//reserved = 0x01,
			//reserved = 0x02,
			//reserved = 0x03,
			Message = 0x04,
		};

		enum class pipe_read_mode : int8_t {
			Byte = 0x00,
			//reserved = 0x01,
			Message = 0x02,
		};

		class named_pipe {
			HANDLE handle;
			bool                created = false;
			SECURITY_ATTRIBUTES m_securityAttributes;

			public:
			named_pipe(os::create_only_t, std::string name, size_t max_instances = PIPE_UNLIMITED_INSTANCES,
					   pipe_type type = pipe_type::Message, pipe_read_mode mode = pipe_read_mode::Message,
					   bool is_unique = false);
			named_pipe(os::create_or_open_t, std::string name, size_t max_instances = PIPE_UNLIMITED_INSTANCES,
					   pipe_type type = pipe_type::Message, pipe_read_mode mode = pipe_read_mode::Message,
					   bool is_unique = false);
			named_pipe(os::open_only_t, std::string name, pipe_read_mode mode = pipe_read_mode::Message);
			~named_pipe();

			os::error available(size_t &avail);

			os::error total_available(size_t &avail);

			[[deprecated("This method is deprecated, use read(char*, size_t, std::shared_ptr<os::async_op>&, os::async_op_cb_t) instead.")]]
			os::error read(std::unique_ptr<os::windows::async_request> &request, char *buffer, size_t buffer_length);
			
			[[deprecated("This method is deprecated, use write(const char*, size_t, std::shared_ptr<os::async_op>&, os::async_op_cb_t) instead.")]]
			os::error write(std::unique_ptr<os::windows::async_request> &request, const char *buffer,
							size_t buffer_length);

			os::error read(char *buffer, size_t buffer_length, std::shared_ptr<os::async_op> &op, os::async_op_cb_t cb);

			os::error write(const char *buffer, size_t buffer_length, std::shared_ptr<os::async_op> &op,
							os::async_op_cb_t cb);

			bool is_created();

			bool is_connected();

			public: // created only
			[[deprecated("This method is deprecated, use accept(std::shared_ptr<os::async_op>&, os::async_op_cb_t) instead.")]]
			os::error accept(std::unique_ptr<os::windows::async_request> &request);

			os::error accept(std::shared_ptr<os::async_op> &op, os::async_op_cb_t cb);
		};
	} // namespace windows
} // namespace os

#endif // OS_WINDOWS_NAMED_PIPE_HPP
