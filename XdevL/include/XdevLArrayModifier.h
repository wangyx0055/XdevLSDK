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

#ifndef XDEVL_ARRAY_MODIFIER_H
#define XDEVL_ARRAY_MODIFIER_H

namespace xdl {
	
	/**
		@class XdevLArrayModifier
		@brief Modifies an array
		
		This class gives easy handling of writing and reading structure from arrays.
	*/
	class XdevLArrayModifier {
		public:

			XdevLArrayModifier(xdl_uint8* buffer, xdl_uint64 size) : m_cursor(0),
				m_buffer(buffer),
				m_size(size) {

			}
			
			/// Returns an unsigned integer 8 bit values from the array.
			xdl_int readU8(xdl_uint8& value) {
				XdevLScopeEndIncrease inc(this->m_cursor, sizeof(xdl_uint8),this-> m_size);

				value = *((xdl_uint8*)&m_buffer[this->m_cursor]);
				
				return ERR_OK;
			}

			/// Returns an integer 8 bit values from the array.
			xdl_int read8(xdl_int8& value) {
				XdevLScopeEndIncrease inc(this->m_cursor, sizeof(xdl_int8), this->m_size);

				value = *((xdl_int8*)&m_buffer[this->m_cursor]);
				
				return ERR_OK;
			}

			/// Returns an unsigned integer 16 bit values from the array.
			xdl_int readU16(xdl_uint16& value) {
				XdevLScopeEndIncrease inc(this->m_cursor, sizeof(xdl_uint16), this->m_size);

				value = *((xdl_uint16*)&m_buffer[this->m_cursor]);
				
				return ERR_OK;
			}

			/// Returns an integer 16 bit values from the array.
			xdl_int read16(xdl_int16& value) {
				XdevLScopeEndIncrease inc(this->m_cursor, sizeof(xdl_int16), this->m_size);

				value = *((xdl_int16*)&m_buffer[this->m_cursor]);
				
				return ERR_OK;
			}

			/// Returns an unsigned integer 32 bit values from the array.
			xdl_int readU32(xdl_uint32& value) {
				XdevLScopeEndIncrease inc(this->m_cursor, sizeof(xdl_uint32), this->m_size);

				value =  *((xdl_uint32*)&m_buffer[this->m_cursor]);
				
				return ERR_OK;
			}

			/// Returns an integer 32 bit value from the array.
			xdl_int read32(xdl_int32& value) {
				XdevLScopeEndIncrease inc(this->m_cursor, sizeof(xdl_int32), this->m_size);

				value = *((xdl_int32*)&m_buffer[this->m_cursor]);
				
				return ERR_OK;
			}

			/// Returns an unsigned integer 64 bit value from the array.
			xdl_int readU64(xdl_uint64& value) {
				XdevLScopeEndIncrease inc(this->m_cursor, sizeof(xdl_uint64), this->m_size);
				
				value = *((xdl_uint64*)&m_buffer[this->m_cursor]);
				
				return ERR_OK;
			}

			/// Returns an integer 64 bit value from the array.
			xdl_int read64(xdl_int64& value) {
				XdevLScopeEndIncrease inc(this->m_cursor, sizeof(xdl_int64), this->m_size);
				
				value = *((xdl_int64*)&m_buffer[this->m_cursor]);
				
				return ERR_OK;
			}

			/// Returns a float values from the array.
			xdl_int readFloat(xdl_float& value) {
				XdevLScopeEndIncrease inc(this->m_cursor, sizeof(xdl_float), this->m_size);

				value = *((xdl_float*)&m_buffer[this->m_cursor]);
				
				return ERR_OK;
			}
			
			/// Returns a double values from the array.
			xdl_int readDouble(xdl_double& value) {
				XdevLScopeEndIncrease inc(this->m_cursor, sizeof(xdl_double), this->m_size);

				value = *((xdl_double*)&m_buffer[this->m_cursor]);
				
				return ERR_OK;
			}
			
			/// Returns values from the array.
			/**
				Will read a value from the current position in the buffer. Everytime
				a value is read, the cursor will be moved to the next memory area.
			*/
			template <typename T>
			xdl_int read(T& value) {
				XdevLScopeEndIncrease inc(this->m_cursor, sizeof(T), this->m_size);

				value = *((T*)&m_buffer[this->m_cursor]);
				
				return ERR_OK;
			}
			template <typename T>
			T read() {
				XdevLScopeEndIncrease inc(this->m_cursor, sizeof(T), this->m_size);
				
				return *((T*)&m_buffer[this->m_cursor]);
			}			
			
			
			/// Puts values to the buffer.
			/**
				Will write the value to the current position in the buffer. Everytime
				a value is writen, the cursor will be moved to the next free memory area.
			*/
			template <typename T>
			void write(T value) {
				XdevLScopeEndIncrease inc(this->m_cursor, sizeof(T), this->m_size);
				
				*((T*)&m_buffer[this->m_cursor]) = value;
			}
			
			void writeStringUTF8(const XdevLString& str) {
				XdevLScopeEndIncrease inc(this->m_cursor, str.length() + 1, this->m_size);
				
				size_t bytes = 0;
				for( ; bytes < str.length()*str.characterSize(); bytes++) {
					*((xdl_uint8*)&m_buffer[this->m_cursor] + bytes) = str.begin()[bytes];
				}
				bytes++;
				*((xdl_uint8*)&m_buffer[this->m_cursor] + bytes) = '\0';
				
			}
			
			XdevLString readStringUTF8() {
				XdevLString str("");
				// TODO we have to check the /0 string for utf8/utf16/utf32.
				xdl_char tmp = '\0';
				do {
					tmp = m_buffer[this->m_cursor++];
					str += tmp;
				}
				while(tmp != '\0');
				
				return str;
			}
			
			/// Reset the pointer
			void reset() {
				this->m_cursor = 0;
			}
			
			/// Returns the size of the array.
			xdl_uint64 getSize() const  {
				return this->m_size;
			}
			
			/// Returns the pointer to the array.
			xdl_uint8* getData() {
				return this->m_buffer;
			}

		private:
			xdl_uint64 	m_cursor;
			xdl_uint8*	m_buffer;
			xdl_uint64 	m_size;
	};
	
}

#endif