/*
	Copyright (c) 2005 - 2016 Cengiz Terzibas

	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in the
	Software without restriction, including without limitation the rights to use, copy,
	modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
	and to permit persons to whom the Software is furnished to do so, subject to the
	following conditions:

	The above copyright notice and this permission notice shall be included in all copies
	or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.

	cengiz@terzibas.de
*/

#ifndef XDEVL_SHARED_MEMORY_UNIX_H
#define XDEVL_SHARED_MEMORY_UNIX_H

#include <XdevLSemaphore.h>

namespace xdl {

	enum class XdevLSharedMemoryCreateType {
	  XDEVL_SHM_CREATE_TYPE_CREATE,
	  XDEVL_SHM_CREATE_TYPE_OPEN,
	  XDEVL_SHM_CREATE_TYPE_UNKOWN
	};

	enum class XdevLSharedMemoryAccess {
	  READ_ONLY,
	  READ_WRITE
	};

	struct XdevLSharedMemoryCreateMode {
	};
	struct XdevLSharedMemoryOpenMode {
	};

	XdevLSharedMemoryCreateMode 	XdevLSharedMemoryCreate;
	XdevLSharedMemoryOpenMode		XdevLSharedMemoryOpen;

	struct XdevLSharedMemoryReadOnlyMode {
	};

	struct XdevLSharedMemoryReadWriteMode {
	};

	XdevLSharedMemoryReadOnlyMode XdevLSharedMemoryReadOnly;
	XdevLSharedMemoryReadWriteMode XdevLSharedMemoryReadWrite;


	/**
	 * @class XdevLSharedMemory
	 * @brief A named shared memory managed by second readers/writers threading problem.
	 * @author Cengiz Terzibas
	 */
	class XdevLSharedMemory {
		public:
			XdevLSharedMemory();

			~XdevLSharedMemory();

			/// Create a shared memory object.
			/**
			 * The shared memory will use READ_WRITE acces mode. So why only support READ_WRITE
			 * and not for example READ_ONLY? Creating a shared memory means, at least for this
			 * class to be the owner of the shared memory. That means if this class is the owner
			 * and is not able to write that makes no sense.
			 * @param create This object will be the owner of the shared memory.
			 * @param name The name of the shared memory.
			 * @param sharedMemorySize The size in bytes of the shared memory.
			 */
			XdevLSharedMemory(XdevLSharedMemoryCreateMode& create, const xdl_char* name, size_t sharedMemorySize) throw();


			XdevLSharedMemory(XdevLSharedMemoryOpenMode& open,
			                  const xdl_char* name,
			                  size_t sharedMemorySize,
			                  XdevLSharedMemoryReadOnlyMode& read_only) throw();

			XdevLSharedMemory(XdevLSharedMemoryOpenMode& open,
			                  const xdl_char* name,
			                  size_t sharedMemorySize,
			                  XdevLSharedMemoryReadWriteMode& read_write) throw();

			xdl_int create(const xdl_char* name, size_t sharedMemorySize);

			// Opens a shared memory either read_only or read_write.
			xdl_int open(const xdl_char* name, xdl_uint sharedMemorySize, XdevLSharedMemoryAccess access_mode);

			/// Close shared memory.
			xdl_int close() throw();

			/// Write bytes into the shared memory.
			/**
			 * The access is thread save.
			 * @param array A valid pointer to the array to read from.
			 * @param size The number of bytes that should be read from
			 * 				array and written into shared memory.
			 */
			void write_ts(xdl_uint8* array, size_t size);

			/// Write bytes into the shared memory.
			/**
			 * The access is not thread save.
			 * @param array A valid pointer to the array to read from.
			 * @param size The number of bytes that should be read from
			 * 				array and written into shared memory.
			 */
			void write(xdl_uint8* array, size_t size);

			/// Read from the shared memory.
			/**
			 * The access is thread save.
			 * @param array A valid pointer to the array to write to.
			 * @param size The number of bytes that should be read from
			 * 				shared memory and written into array.
			 */
			void read_ts(xdl_uint8* array, size_t size);

			/// Read from the shared memory.
			/**
			 * The access is not thread save.
			 * @param array A valid pointer to the array to write to.
			 * @param size The number of bytes that should be read from
			 * 				shared memory and written into array.
			 */
			void read(xdl_uint8* array, size_t size);

			/// The a pointer to access directly to the shared memory.
			xdl_uint8* data();

			xdl_int getNativeHandle() const;

		private:

			// Holds the name of the shared memory.
			std::string m_name;

			// Holds the size of the shared memory.
			size_t m_size;

			// Second readers/writers problem variables.
			XdevLSemaphore 	m_writer;
			XdevLSemaphore 	m_reader;
			XdevLSemaphore 	m_order;
			xdl_uint64		m_readercount;
			xdl_uint64		m_previous;
			xdl_uint64		m_current;

			// The shared memory.
			xdl_int m_smfd;

			// The mapped memory area.
			xdl_uint8* m_mapped_region;
			XdevLSharedMemoryCreateType m_creationType;
	};

}

#endif
