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
		@brief A Ringbuffer using the heap memory.
		
		This ringbuffer is not thread safe. 
	*/
	template<typename _T, int _size>
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
			// Put one element into the ring buffer.
			/**
				@param element A valid element to put into the ring buffer.
				@return Returns -1 if a buffer overflow appears else 0.
			*/
			int put(_T& element) {
				b[wp] = element;
				wp = (wp + 1) % size;
				if(fsize == size) {
					rp = (rp + 1) % size;
					return -1;
				} else {
					fsize++;
				}
				return 0;
			}

			// Pull from the front one element from the ring buffer.
			/**
				@param dest A valid _T object which will be filled with the accuired ring buffer element.
				@return Returns -1 if a buffer underflow appears else 0.
			*/
			int pull_front(_T& dest) {
				if(fsize == 0) {
					return -1;
				}
				dest = b[rp];
				rp = (rp + 1) % size;
				fsize--;
				return 0;
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
