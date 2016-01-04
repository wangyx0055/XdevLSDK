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

#ifndef XDEVL_RINGBUFFER_H
#define XDEVL_RINGBUFFER_H

namespace xdl {

	/**
		@class XdevLRingBuffer
		@tparam _T The type/structure to store in the ringbuffer.
		@tparam _size The size of the ringbuffer.

		A ring/circular buffer is a sing and fixed sized array of a specified structure. The beginning and end of the buffer is connected.
		Check the internet for more information.

		@note This ringbuffer is not thread safe.

		@section usage Usage

		@code

		XdevLRingbuffer<float, 32> buffer;

		...

		while(...) {
			...
			buffer.put(value);

			...

			float tmp;
			if(buffer.pull_front(tmp) == ERR_BUFFER_OVERFLOW) {
				// Overflow happened.
			}
			..
		}


		@endcode
	*/
	template<typename _T, xdl_int _size>
	class XdevLRingBuffer {
		public:
			XdevLRingBuffer() : size(_size) {
				b 		= new _T[_size];
				rp 		= 0;
				wp 		= 0;
				fsize = 0;
			}

			~XdevLRingBuffer() {
				delete [] b;
			};

			/// Put one element into the ring buffer.
			/**
				@param element A valid element to put into the ring buffer.
				@return Returns ERR_BUFFER_OVERFLOW if a buffer overflow appears else ERR_OK.
			*/
			xdl_int put(_T& element) {
				b[wp] = element;
				wp = (wp + 1) % size;
				if(fsize == size) {
					rp = (rp + 1) % size;
					return ERR_BUFFER_OVERFLOW;
				} else {
					fsize++;
				}
				return ERR_OK;
			}

			/// Pull from the front one element from the ring buffer.
			/**
				@param dest A valid _T object which will be filled with the acquired ring buffer element.
				@return Returns ERR_BUFFER_UNDERFLOW if a buffer underflow appears else 0.
			*/
			xdl_int pull_front(_T& dest) {
				if(fsize == 0) {
					return ERR_BUFFER_UNDERFLOW;
				}
				dest = b[rp];
				rp = (rp + 1) % size;
				fsize--;
				return ERR_OK;
			}

			size_t get_fill_size() {
				return fsize;
			}

		private:

			xdl_int64 size;
			xdl_int64 rp;
			xdl_int64 wp;
			xdl_int64 fsize;
			_T* b;

	};

}

#endif
