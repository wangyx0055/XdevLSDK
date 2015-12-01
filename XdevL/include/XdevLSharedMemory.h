#ifndef XDEVL_SHARED_MEMORY_H
#define XDEVL_SHARED_MEMORY_H

#include <XdevLSemaphore.h>

namespace xdl {

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
			XdevLSharedMemory() {}
			~XdevLSharedMemory() {
				close();
			}

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
			XdevLSharedMemory(XdevLSharedMemoryCreateMode& create, const xdl_char* name, size_t sharedMemorySize) throw() :
				m_name(name), 
				m_size(sharedMemorySize) {

					if(this->create(name, sharedMemorySize) != ERR_OK) {
						exit(-1);
					}

			}


			XdevLSharedMemory(XdevLSharedMemoryOpenMode& open,
			                  const xdl_char* name,
			                  size_t sharedMemorySize,
			                  XdevLSharedMemoryReadOnlyMode& read_only) throw() : m_name(name), m_size(sharedMemorySize) {
				if(this->open(name, sharedMemorySize, XdevLSharedMemoryAccess::READ_ONLY) != ERR_OK) {
					exit(-1);
				}
			}

			XdevLSharedMemory(XdevLSharedMemoryOpenMode& open,
			                  const xdl_char* name,
			                  size_t sharedMemorySize,
			                  XdevLSharedMemoryReadWriteMode& read_write) throw() : m_name(name), m_size(sharedMemorySize)  {
				if(this->open(name, sharedMemorySize, XdevLSharedMemoryAccess::READ_WRITE) != ERR_OK) {
					exit(-1);
				}
			}

			xdl_int create(const xdl_char* name, size_t sharedMemorySize) {

				// Create semaphore names.
				std::string writers_name(name);
				writers_name += ".wsemaphore";
				std::string readers_name(name);
				readers_name += ".rsemaphore";
				std::string orders_name(name);
				orders_name += ".osemaphore";

#if XDEVL_PLATFORM_UNIX
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

				m_mapped_region = (xdl_uint8*)mmap(NULL, sharedMemorySize, PROT_READ | PROT_WRITE, MAP_SHARED, m_smfd, 0);
				if(m_mapped_region == MAP_FAILED) {
					std::cerr << "XdevLSharedMemory::XdevLSharedMemory: mmap failed." << std::endl;
					return ERR_ERROR;
				}
#endif

				// Open all semaphores to synronize data between processes.
				m_writer 	= XdevLSemaphore(XdevLSemaphoreCreate, writers_name.c_str(), 1);
				m_reader 	= XdevLSemaphore(XdevLSemaphoreCreate, readers_name.c_str(), 1);
				m_order		= XdevLSemaphore(XdevLSemaphoreCreate, orders_name.c_str(), 1);

				return ERR_OK;
			}

			// Opens a shared memory either read_only or read_write.
			xdl_int open(const xdl_char* name, xdl_uint sharedMemorySize, XdevLSharedMemoryAccess access_mode) {

				// Create semaphore names.
				std::string writers_name = m_name + ".wsemaphore";
				std::string readers_name = m_name + ".rsemaphore";
				std::string orders_name = m_name + ".osemaphore";

#if defined (XDEVL_PLATFORM_UNIX)
				int mode = 0;
#endif
				switch(access_mode) {
					case XdevLSharedMemoryAccess::READ_ONLY: {
#if defined (XDEVL_PLATFORM_UNIX)
						mode = O_RDONLY;
#endif
					}
					break;
					case XdevLSharedMemoryAccess::READ_WRITE: {
#if defined (XDEVL_PLATFORM_UNIX)
						mode = O_RDWR;
#endif
					}
					break;
					default:
						break;
				}
#if defined (XDEVL_PLATFORM_UNIX)
				m_smfd = shm_open(m_name.c_str(),  O_EXCL | mode, S_IRWXU | S_IRWXG);
				if(m_smfd < 0) {
					std::cerr << "XdevLSharedMemory::XdevLSharedMemory: shm_open failed." << std::endl;
					return ERR_ERROR;
				}

				//Map the whole shared memory in this process
				m_mapped_region = (xdl_uint8*)mmap(NULL, sharedMemorySize, PROT_READ | PROT_WRITE, MAP_SHARED, m_smfd, 0);
				if(m_mapped_region == nullptr) {
					std::cerr << "XdevLSharedMemory::open: mmap failed." << std::endl;
					return ERR_ERROR;
				}
#endif


				// Open all semaphores to synronize data between processes.
				m_writer 	= XdevLSemaphore(XdevLSemaphoreOpen, writers_name.c_str(), 1);
				m_reader 	= XdevLSemaphore(XdevLSemaphoreOpen, readers_name.c_str(), 1);
				m_order		= XdevLSemaphore(XdevLSemaphoreOpen, orders_name.c_str(), 1);

				return ERR_OK;
			}

			/// Close shared memory.
			xdl_int close() throw() {

				// Close all semaphores. If we wouldn't do that we would have dead locks.
				m_writer.close();
				m_reader.close();
				m_order.close();

				if(shm_unlink(m_name.c_str()) == -1) {
					std::cerr << "XdevLSharedMemory::XdevLSharedMemory: shm_unlink failed." << std::endl;
					return ERR_ERROR;
				}

				munmap(m_mapped_region, m_size);

				return ERR_OK;
			}

			/// Write bytes into the shared memory.
			/**
			 * The access is thread save.
			 * @param array A valid pointer to the array to read from.
			 * @param size The number of bytes that should be read from
			 * 				array and written into shared memory.
			 */
			void write_ts(xdl_uint8* array, size_t size) {
				// Check if the size the user like to write is bigger
				// then the size of the shared memory.
				assert(m_mapped_region == nullptr);

				m_order.wait();

				m_writer.wait();

				m_order.wait();

				// Everyting OK, so let's write it into the buffer.
				memmove((void*)m_mapped_region, (void*)array, size);

				m_writer.post();
			}

			/// Write bytes into the shared memory.
			/**
			 * The access is not thread save.
			 * @param array A valid pointer to the array to read from.
			 * @param size The number of bytes that should be read from
			 * 				array and written into shared memory.
			 */
			void write(xdl_uint8* array, size_t size) {
				// Check if the size the user like to write is bigger
				// then the size of the shared memory.
				assert(m_size >= size);

				// Everyting OK, so let's write it into the buffer.
				memmove((void*)m_mapped_region, (void*)array, size);

			}

			/// Read from the shared memory.
			/**
			 * The access is thread save.
			 * @param array A valid pointer to the array to write to.
			 * @param size The number of bytes that should be read from
			 * 				shared memory and written into array.
			 */
			void read_ts(xdl_uint8* array, size_t size) {
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

			/// Read from the shared memory.
			/**
			 * The access is not thread save.
			 * @param array A valid pointer to the array to write to.
			 * @param size The number of bytes that should be read from
			 * 				shared memory and written into array.
			 */
			void read(xdl_uint8* array, size_t size) {
				// Check if the size the user like to write is bigger
				// then the size of the shared memory.
				assert(m_size >= size);

				// Everyting OK, so let's read from the buffer.
				memmove((void*)array, (void*)m_mapped_region, size);

			}

			/// The a pointer to access directly to the shared memory.
			xdl_uint8* data() {
				return (xdl_uint8*)m_mapped_region;
			}

#if XDEVL_PLATFORM_UNIX
			xdl_int getNativeHandle() const { return m_smfd;}
#endif
			
		private:
			// Holds the name of the shared memory.
			std::string m_name;

			// Holds the size of the shared memory.
			size_t 			m_size;

			// Second readers/writers problem variables.
			XdevLSemaphore 		m_writer;
			XdevLSemaphore 		m_reader;
			XdevLSemaphore 		m_order;
			xdl_uint64				m_readercount;
			xdl_uint64				m_previous;
			xdl_uint64				m_current;

			// The shared memory.
#if defined (XDEVL_PLATFORM_UNIX)
			xdl_int m_smfd;
#endif

			// The mapped memory area.
			xdl_uint8* 				m_mapped_region;
	};

}

#endif
