/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2015 Cengiz Terzibas

	This library is free software; you can redistribute it and/or modify it under the
	terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2.1 of the License, or (at your option) any later version.
	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
	Suite 330, Boston, MA 02111-1307 USA

	I would appreciate if you report all bugs to: cengiz@terzibas.de
*/

#ifndef XDEVL_SEMAPHORE_H
#define XDEVL_SEMAPHORE_H

namespace xdl {

	/// Parameter to create the sempahore
	struct XdevLSemaphoreModeCreate {
	};

	/// Parameter to open a semaphore
	struct XdevLSemaphoreModeOpen {
	};

	static XdevLSemaphoreModeCreate XdevLSemaphoreCreate;
	static XdevLSemaphoreModeOpen 	XdevLSemaphoreOpen;

	/**
	 * @enum CreationMode
	 * @brief Creating modes.
	 */
	enum CreationMode {
	  OPEN, 		/// Only start in open mode. Opener is not the owner.
	  CREATE		/// Create the semaphore. Creator will be the owner.
	};

	/**
	 * @class XdevLSemaphore
	 * @brief Semaphore
	 * @author Cengiz Terzibas
	 *
	 * The semaphore is a mutex on the kernel level. Two different processes can access the semaphore to sync
	 * each other.
	 */
	class XdevLSemaphore {
		public:

			XdevLSemaphore() {};

			~XdevLSemaphore() throw() {};

			/// Create the semaphore.
			/**
			 * @param create The owner the the semaphore will be this object.
			 * @param name Name of the semaphore.
			 * @param value The initial value for the semaphore.
			 *
			 * Throws exception if not possible.
			 */
			XdevLSemaphore(XdevLSemaphoreModeCreate& create,
			               const xdl_char* name,
			               xdl_uint value) throw() : m_name(name) {

				if(this->create(name, value) != ERR_OK) {
					exit(-1);
				}
			}
			/// Open the semaphore.
			/**
			 * @param open The owner of the semaphore will be another object.
			 * @param name Name of the semaphore.
			 * @param value The initial value for the semaphore.
			 *
			 * Throws exception if not possible.
			 */
			XdevLSemaphore(	XdevLSemaphoreModeOpen& open,
			                const xdl_char* name,
			                xdl_uint value) throw() : m_name(name) {
					if(this->open(name, value) != ERR_OK) {
						exit(-1);
					}
			}
			
			/// Open a named semaphore.
			xdl_int open(const xdl_char* name, xdl_uint value) {
				m_name = name;
#if defined (XDEVL_PLATFORM_UNIX)
				this->m_smp = sem_open(m_name.c_str(), 0, 0644, value);
				if(this->m_smp == SEM_FAILED) {
						std::cerr << "XdevLSemaphore::XdevLSemaphore: sem_open failed." << std::endl;
						sem_unlink(m_name.c_str());
						return ERR_ERROR;
					}
#endif
				return ERR_OK;
			}
			
			/// Create a named semaphore.
			xdl_int create(const xdl_char* name, xdl_uint value) {
				m_name = name;
#if defined (XDEVL_PLATFORM_UNIX)
				this->m_smp = sem_open(m_name.c_str(), O_CREAT, 0644, value);
				if(this->m_smp == SEM_FAILED)
					{
						std::cerr << "XdevLSemaphore::XdevLSemaphore: sem_open failed." << std::endl;
						sem_unlink(m_name.c_str());
						return ERR_ERROR;
					}
#endif
				return ERR_OK;
			}

			/// Close the semaphore.
			void close() throw() {
#if defined (XDEVL_PLATFORM_UNIX)			
				sem_close(this->m_smp);
				sem_unlink(this->m_name.c_str());
#endif
			}

			/// Lock the mutex.
			void wait() throw() {
#if defined (XDEVL_PLATFORM_UNIX)					
				 sem_wait(this->m_smp);
#endif
			}

			/// Unlock the mutex.
			void post() throw() {
#if defined (XDEVL_PLATFORM_UNIX)					
				 sem_post(this->m_smp);
#endif
			}

		private:
#if defined (XDEVL_PLATFORM_UNIX)
			sem_t* m_smp;
#endif
			std::string m_name;
	};


}


#endif
