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
			XdevLRingBuffer() :
				size(_size),
				rp(0),
				wp(0),
				b(nullptr) {
				resize(_size);
			}

			~XdevLRingBuffer() {
				destroy();
			};

			/// Resize the ringbufer.
			/**
				Resizing will detroy all elements in the ringbuffer. So before you resize it
				make sure you used all elements within.

				@param size The new size of the ringbuffer.
			*/
			xdl_int resize(xdl_int size) {
				destroy();

				b 		= new _T[_size];
				rp 		= 0;
				wp 		= 0;
				fsize = 0;
				return ERR_OK;
			}

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

			/// Returns the current fill size of the buffer.
			size_t get_fill_size() {
				return fsize;
			}

			/// Returns the size of the buffer.
			size_t get_size() {
				return _size;
			}

		private:

			/// Destroy the ringbuffer.
			void destroy() {
				if(nullptr != b) {
					delete [] b;
					rp 		= 0;
					wp 		= 0;
					fsize = 0;
				}
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
