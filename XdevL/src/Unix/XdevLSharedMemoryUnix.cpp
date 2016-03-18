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

#include <string.h>
#include <cassert>

#include <XdevLSharedMemory.h>
#include <XdevLSemaphore.h>
#include <XdevLError.h>
#include <XdevLTypes.h>



namespace xdl {

	XdevLSharedMemory::XdevLSharedMemory() :
		m_creationType(XdevLSharedMemoryCreateType::XDEVL_SHM_CREATE_TYPE_UNKOWN) {
	}

	XdevLSharedMemory::~XdevLSharedMemory() {
		close();
	}

	XdevLSharedMemory::XdevLSharedMemory(XdevLSharedMemoryCreateMode& create, const xdl_char* name, size_t sharedMemorySize) throw() :
		m_name(name),
		m_size(sharedMemorySize) {

		if(this->create(name, sharedMemorySize) != ERR_OK) {
			exit(-1);
		}
		m_creationType = XdevLSharedMemoryCreateType::XDEVL_SHM_CREATE_TYPE_CREATE;
	}


	XdevLSharedMemory::XdevLSharedMemory(XdevLSharedMemoryOpenMode& open,
	                                     const xdl_char* name,
	                                     size_t sharedMemorySize,
	                                     XdevLSharedMemoryReadOnlyMode& read_only) throw() : m_name(name), m_size(sharedMemorySize) {
		if(this->open(name, sharedMemorySize, XdevLSharedMemoryAccess::READ_ONLY) != ERR_OK) {
			exit(-1);
		}
		m_creationType = XdevLSharedMemoryCreateType::XDEVL_SHM_CREATE_TYPE_OPEN;
	}

	XdevLSharedMemory::XdevLSharedMemory(XdevLSharedMemoryOpenMode& open,
	                                     const xdl_char* name,
	                                     size_t sharedMemorySize,
	                                     XdevLSharedMemoryReadWriteMode& read_write) throw() : m_name(name), m_size(sharedMemorySize)  {
		if(this->open(name, sharedMemorySize, XdevLSharedMemoryAccess::READ_WRITE) != ERR_OK) {
			exit(-1);
		}
		m_creationType = XdevLSharedMemoryCreateType::XDEVL_SHM_CREATE_TYPE_OPEN;
	}

	xdl_int XdevLSharedMemory::create(const xdl_char* name, size_t sharedMemorySize) {

		// Create semaphore names.
		std::string writers_name(name);
		writers_name += ".wsemaphore";
		std::string readers_name(name);
		readers_name += ".rsemaphore";
		std::string orders_name(name);
		orders_name += ".osemaphore";

		if(shm_unlink(m_name.c_str()) == -1) {
			// If this fails here we don't bother because it is ok.
			//std::cerr << "XdevLSharedMemory::XdevLSharedMemory: shm_unlink failed." << std::endl;
		}

		m_smfd = shm_open(m_name.c_str(), O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG);
		if(m_smfd < 0) {
			perror("In shm_open()");
			std::cerr << "XdevLSharedMemory::XdevLSharedMemory: shm_open failed." << std::endl;
			return ERR_ERROR;
		}

		// Adjusting mapped file size (make room for the whole segment to map)
		if(ftruncate(m_smfd, sharedMemorySize) < 0) {
			std::cerr << "XdevLSharedMemory::XdevLSharedMemory: ftruncate failed." << std::endl;
			return ERR_ERROR;
		}

		m_mapped_region = (xdl_uint8*)mmap(nullptr, sharedMemorySize, PROT_READ | PROT_WRITE, MAP_SHARED, m_smfd, 0);
		if(m_mapped_region == MAP_FAILED) {
			std::cerr << "XdevLSharedMemory::XdevLSharedMemory: mmap failed." << std::endl;
			return ERR_ERROR;
		}

		// Open all semaphores to synronize data between processes.
		m_writer 	= XdevLSemaphore(XdevLSemaphoreCreate, writers_name.c_str(), 1);
		m_reader 	= XdevLSemaphore(XdevLSemaphoreCreate, readers_name.c_str(), 1);
		m_order		= XdevLSemaphore(XdevLSemaphoreCreate, orders_name.c_str(), 1);

		return ERR_OK;
	}

	xdl_int XdevLSharedMemory::open(const xdl_char* name, xdl_uint sharedMemorySize, XdevLSharedMemoryAccess access_mode) {

		// Create semaphore names.
		std::string writers_name = m_name + ".wsemaphore";
		std::string readers_name = m_name + ".rsemaphore";
		std::string orders_name = m_name + ".osemaphore";

		int mode = 0;
		int mprot = PROT_NONE;

		switch(access_mode) {
			case XdevLSharedMemoryAccess::READ_ONLY: {

				mode = O_RDONLY;
				mprot = PROT_READ;

			}
			break;
			case XdevLSharedMemoryAccess::READ_WRITE: {

				mode = O_RDWR;
				mprot = PROT_READ | PROT_WRITE;

			}
			break;
			default:
				break;
		}

		m_smfd = shm_open(m_name.c_str(),  O_EXCL | mode, S_IRWXU | S_IRWXG);
		if(m_smfd < 0) {
			std::cerr << "XdevLSharedMemory::XdevLSharedMemory: shm_open failed." << std::endl;
			return ERR_ERROR;
		}

		//Map the whole shared memory in this process
		m_mapped_region = (xdl_uint8*)mmap(nullptr, sharedMemorySize, mprot, MAP_SHARED, m_smfd, 0);
		if(m_mapped_region == nullptr) {
			std::cerr << "XdevLSharedMemory::open: mmap failed." << std::endl;
			return ERR_ERROR;
		}

		// Open all semaphores to synronize data between processes.
		m_writer 	= XdevLSemaphore(XdevLSemaphoreOpen, writers_name.c_str(), 1);
		m_reader 	= XdevLSemaphore(XdevLSemaphoreOpen, readers_name.c_str(), 1);
		m_order		= XdevLSemaphore(XdevLSemaphoreOpen, orders_name.c_str(), 1);

		return ERR_OK;
	}

	xdl_int XdevLSharedMemory::close() throw() {

		// Close all semaphores. If we wouldn't do that we would have dead locks.
		m_writer.close();
		m_reader.close();
		m_order.close();

		if(m_creationType == XdevLSharedMemoryCreateType::XDEVL_SHM_CREATE_TYPE_CREATE)
			if(shm_unlink(m_name.c_str()) == -1) {
				std::cerr << "XdevLSharedMemory::XdevLSharedMemory: shm_unlink failed." << std::endl;
				return ERR_ERROR;
			}

		munmap(m_mapped_region, m_size);

		return ERR_OK;
	}

	void XdevLSharedMemory::write_ts(xdl_uint8* array, size_t size) {
		// Check if the size the user like to write is bigger
		// then the size of the shared memory.
		assert(m_mapped_region != nullptr);

		m_order.wait();

		m_writer.wait();

		m_order.wait();

		// Everyting OK, so let's write it into the buffer.
		memmove((void*)m_mapped_region, (void*)array, size);

		m_writer.post();
	}

	void XdevLSharedMemory::write(xdl_uint8* array, size_t size) {
		// Check if the size the user like to write is bigger
		// then the size of the shared memory.
		assert(m_size >= size);

		// Everyting OK, so let's write it into the buffer.
		memmove((void*)m_mapped_region, (void*)array, size);

	}

	void XdevLSharedMemory::read_ts(xdl_uint8* array, size_t size) {
		// Check if the size the user like to write is bigger
		// then the size of the shared memory.
		//	assert(m_mapped_region->get_size() >= size);

		m_order.wait();

		m_reader.wait();

		m_previous = m_readercount;
		m_readercount++;

		m_reader.post();

		if(m_previous == 0) {
			m_writer.wait();
		}
		m_order.post();

		// Everyting Save here, so let's read from the buffer.
		memmove((void*)array, (void*)m_mapped_region, size);

		m_reader.wait();

		m_readercount--;
		m_current = m_readercount;

		m_reader.post();
		if(m_current == 0) {
			m_writer.post();
		}

	}

	void XdevLSharedMemory::read(xdl_uint8* array, size_t size) {
		// Check if the size the user like to write is bigger
		// then the size of the shared memory.
		assert(m_size >= size);

		// Everyting OK, so let's read from the buffer.
		memmove((void*)array, (void*)m_mapped_region, size);

	}

	xdl_uint8* XdevLSharedMemory::data() {
		return (xdl_uint8*)m_mapped_region;
	}

	xdl_int XdevLSharedMemory::getNativeHandle() const {
		return m_smfd;
	}


}
