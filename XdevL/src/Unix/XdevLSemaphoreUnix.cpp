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

#include <XdevLSemaphore.h>
#include <XdevLUtils.h>
#include <XdevLError.h>

namespace xdl {

			XdevLSemaphore::XdevLSemaphore() {}

			XdevLSemaphore::~XdevLSemaphore() throw() {}

			XdevLSemaphore::XdevLSemaphore(XdevLSemaphoreModeCreate& create,
			               const xdl_char* name,
			               xdl_uint value) throw() : m_name(name) {

				if(this->create(name, value) != ERR_OK) {
					exit(-1);
				}
			}

			XdevLSemaphore::XdevLSemaphore(	XdevLSemaphoreModeOpen& open,
			                const xdl_char* name,
			                xdl_uint value) throw() : m_name(name) {
				if(this->open(name, value) != ERR_OK) {
					exit(-1);
				}
			}

			xdl_int XdevLSemaphore::open(const xdl_char* name, xdl_uint value) {
				m_name = name;
				this->m_smp = sem_open(m_name.c_str(), 0, 0644, value);
				if(this->m_smp == SEM_FAILED) {
					std::cerr << "XdevLSemaphore::XdevLSemaphore: sem_open failed." << std::endl;
					sem_unlink(m_name.c_str());
					return ERR_ERROR;
				}
				return ERR_OK;
			}

			xdl_int XdevLSemaphore::create(const xdl_char* name, xdl_uint value) {
				m_name = name;
				this->m_smp = sem_open(m_name.c_str(), O_CREAT, 0644, value);
				if(this->m_smp == SEM_FAILED) {
					XDEVL_MODULEX_ERROR(XdevLSemaphore, "XdevLSemaphore::XdevLSemaphore: sem_open failed.")
					sem_unlink(m_name.c_str());
					return ERR_ERROR;
				}
				return ERR_OK;
			}

			void XdevLSemaphore::close() throw() {
				sem_close(this->m_smp);
				sem_unlink(this->m_name.c_str());
			}

			void XdevLSemaphore::wait() throw() {
				sem_wait(this->m_smp);
			}

			void XdevLSemaphore::post() throw() {
				sem_post(this->m_smp);
			}

}
