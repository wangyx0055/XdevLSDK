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

#ifndef XDEVL_UTILS_H
#define XDEVL_UTILS_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>

#include <XdevLTypes.h>
#include <XdevLVersion.h>
#include <tinyxml.h>

namespace xdl {

	inline void XdevLAssertGeneric(const xdl_char *exp, const xdl_char *filename, xdl_int line) {
		printf("================================================\n");
		printf("ASSERTION FAILED!\nExpr: (%s)\nFile: %s\nLine: %d\n", exp, filename, line);
		printf("================================================\n");

		exit(1);
	}
#define XDEVL_ASSERT(exp, message)	if ( !(exp) ) XdevLAssertGeneric( message, __FILE__, __LINE__ )

#ifdef _LOG
	#define XDEVL_MODULEX_SUCCESS(MODULE_NAME, errMsg)	{std::cerr << "\033[1;32m>> " << #MODULE_NAME << ": " << errMsg << "\033[0m";}
	#define XDEVL_MODULEX_ERROR(MODULE_NAME, errMsg)		{std::cerr << "\033[1;31m## " << #MODULE_NAME << ": " << __func__ << ": " << __LINE__  << ": " << errMsg << "\033[0m";}
	#define XDEVL_MODULEX_WARNING(MODULE_NAME, errMsg)	{std::cerr << "\033[1;35m!! " << #MODULE_NAME << ": " << __func__ << ": " << __LINE__ << ": " << errMsg << "\033[0m";}
	#define XDEVL_MODULEX_INFO(MODULE_NAME, errMsg)		{std::cerr << #MODULE_NAME << ": " << errMsg;}

	#define XDEVL_MODULE_SUCCESS(errMsg)	{std::cerr << "\033[1;32m>> " << this->getDescriptor().getName() << ": " << errMsg << "\033[0m";}
	#define XDEVL_MODULE_ERROR(errMsg)		{std::cerr << "\033[1;31m## " << this->getDescriptor().getName() << ": " << __func__ << ": " << __LINE__  << ": " << errMsg << "\033[0m";}
	#define XDEVL_MODULE_WARNING(errMsg)	{std::cerr << "\033[1;35m!! " << this->getDescriptor().getName() << ": " << __func__ << ": " << __LINE__ << ": " << errMsg << "\033[0m";}
	#define XDEVL_MODULE_INFO(errMsg)		{std::cerr << this->getDescriptor().getName() << ": " << errMsg;}

#else
#define XDEVL_MODULE_SUCCESS(errMsg)
#define XDEVL_MODULE_ERROR(errMsg)
#define XDEVL_MODULE_WARNING(errMsg)
#define XDEVL_MODULE_INFO(errMsg)
#endif

	// Returns the module information. (Path, Plugins Name, Version)
	xdl_int getPluginInformationFromFilename(
	  std::string& path,
	  std::string& pluginName,
	  XdevLVersion& version,
	  const std::string& filename);

	/**
		@class XdevLFreeList
		@brief Freelist implementation

		Free lists supports really fast memory management for predefined object types.
		If you create a free list you specify the number and type of that object.

		With the New()function you get an pointer to the new allocated memory. With
		the Delete() you delete that object again. You don't have to think about
		the New() and Delete() functions because they are really fast.
	*/
	template <class T>
	class XdevLFreeList {
		public:
			XdevLFreeList(xdl_int iNumObjects) {
				m_objectBuffer = new T[iNumObjects];
				m_freeObjects = new T*[iNumObjects];

				m_numObjects = iNumObjects;
				m_bFreeOnDestroy = true;

				FreeAll();
			}

			// Constructs a XdevLFreeList with the specified number
			// of objects available for allocation from pre-allocated
			// memory referenced by "pObjectData" and "ppFreeObjects".
			// Note that pObjectData and ppFreeObjects must have
			// "iNumObjects" elements each.
			XdevLFreeList(T *pObjectData, T **ppFreeObjects, xdl_int iNumObjects) {

				m_objectBuffer = pObjectData;
				m_freeObjects = ppFreeObjects;

				m_numObjects = iNumObjects;
				m_bFreeOnDestroy = false;

				FreeAll();
			}

			~XdevLFreeList() {
				// If we have allocated memory,
				// then we must free it.
				if(m_bFreeOnDestroy) {
					delete [] m_freeObjects;
					delete [] m_objectBuffer;
				}
			}

			/// Returns a pointer to a free instance of T.
			T* New() {
				return m_freeObjects[--m_iTop];
			}

			/// Reclaims the instance referenced by pInstance.
			void Delete(T *pInstance) {
				m_freeObjects[m_iTop++] = pInstance;
			}

			/// Makes all instances available for allocation.
			void FreeAll() {
				xdl_int iIndex = (m_numObjects-1);

				for(m_iTop = 0; m_iTop < m_numObjects; m_iTop++) {
					m_freeObjects[m_iTop] = &(m_objectBuffer[iIndex--]);
				}
			}

			/// Returns the total number of objects managed by this XdevLFreeList.
			xdl_int GetSize() {
				return m_numObjects;
			}

			/// Returns the number of instances available for allocation.
			xdl_int GetFree() {
				return m_iTop;
			}

		private:
			// Points to the array of objects.
			T *m_objectBuffer;
			// The number of objects in m_objectBuffer.
			xdl_int m_numObjects;
			// Points to an array of pointers to free
			// objects within m_objectBuffer.  Used as
			// a fixed sized stack.
			T **m_freeObjects;
			// Keeps track of the first available object in
			// m_freeObjects (the top of the stack).
			xdl_int m_iTop;
			// Remembers weather or not to free memory on
			// destruction.
			xdl_bool m_bFreeOnDestroy;
	};

	void sleep(xdl_double pSec);

	/**
		@enum XdevLArrayElement
		@brief Describes the type of an element in a structure.
	*/
	enum XdevLArrayElement {
	  ARRAY_UINT8_ELEMENT					= 0,
	  ARRAY_UCHAR_ELEMENT					= 1,
	  ARRAY_INT8_ELEMENT					= 2,
	  ARRAY_CHAR_ELEMENT					= 4,
	  ARRAY_UINT16_ELEMENT				= 8,
	  ARRAY_INT16_ELEMENT					= 16,
	  ARRAY_UINT32_ELEMENT				= 32,
	  ARRAY_INT32_ELEMENT					= 64,
	  ARRAY_UINT64_ELEMENT				= 128,
	  ARRAY_INT64_ELEMENT					= 256,
	  ARRAY_FLOAT_ELEMENT					= 512,
	  ARRAY_DOUBLE_ELEMENT				= 1024,
	  ARRAY_UNKNOWN_ELEMENT				= 2048
	};

	/**
		@class XdevLArrayItem
		@brief Describes one element in a structure.
	*/
	class XdevLArrayItem {
		public:
			XdevLArrayItem() : element(ARRAY_UNKNOWN_ELEMENT), number(0) {}
			XdevLArrayItem(XdevLArrayElement el, xdl_int32 num) : element(el), number(num) {}
			xdl_uint16 element;
			xdl_int32 number;
	};

	/**
		@class XdevLArrayDeclaration
		@brief Describes the structure of an array.

		Let's say we have a structure which looks like this:
		@code
		struct myStruct{
			xdl::xdl_float x;
			xdl::xdl_float y;
			xdl::xdl_int32 counter;
		}
		@endcode
		And now let's say we want to send such a structure using the socket class. The xdl::XdevLUDPSocket2 class
		allows to send platform independent data. It will take care of big and little endian architecture.
		To do that we need a description of the structure we want do send. Let's create one.
		@code
			xdl::XdevLArrayDeclaration array_decl;
			// The first element of the structure above is a float. So let's add a float.
			array_decl.add(xdl::ARRAY_FLOAT_ELEMENT);
			// The second element of the structure above is also a float.
			array_decl.add(xdl::ARRAY_FLOAT_ELEMENT);
			// The last element of the structure is a integer.
			array_decl.add(xdl::ARRAY_UINT32_ELEMENT);
		@endcode
		So that's it. We have now a array description. We can use this in the XdevLUDPSocket2 class now.

	*/
	class XdevLArrayDeclaration {
		public:

			/// Adds one new array element.
			/**
				@param element Is a XdevLArrayElement
			*/
			void add(const XdevLArrayElement& element) {
				XdevLArrayItem item(element, 1);
				m_elements.push_back(item);
			}

			/// Adds new array elements.
			/**
				If you have more than just one array element from the same type you can use this function
				to prevent multiple calls of the add() function. Let's use the structure from above. We
				have two xdl_floats and one xdl_int32. We can do the following to add two floats:
				@code
				xdl::XdevLArrayDeclaration array_decl;
				// The first element of the structure above is a float. So let's add a float.
				array_decl.add(xdl::ARRAY_FLOAT_ELEMENT,2);
				// The last element of the structure is a integer.
				array_decl.add(xdl::ARRAY_UINT32_ELEMENT);
				@endcode

				@param element Is a xdl::XdevLArrayElement.
				@param number Number of the same type of elements in a row.
			*/
			void add(const XdevLArrayElement& element, xdl_int32 number) {
				XdevLArrayItem item(element, number);
				m_elements.push_back(item);
			}

			/// Returns the number of elements in the array descriptor.
			/**
				@return Returns the number of elements in the description.
			*/
			xdl_int getNumber() const {
				return static_cast<xdl_int>(m_elements.size());
			}

			/// Returns one element.
			/**
				If you want to know which type the 3'rd element in the description from above is
				you can use this function.
				@code
				xdl::XdevLArrayItem item = array_decl.get(2);
				@endcode

				@return Returns the type of idx in the description.
			*/
			XdevLArrayItem get(xdl_int idx) const {
				return m_elements[idx];
			}

			/// Returns the size of the array
			/**
				@return Returns the size of the array in bytes.
			*/
			xdl_int64 getSize() const {
				xdl_int64 size = 0;
				for(size_t c = 0; c < m_elements.size(); ++c) {
					switch(m_elements[c].element) {
						case ARRAY_UINT8_ELEMENT:
						case ARRAY_INT8_ELEMENT:
							size+=sizeof(xdl_int8)*m_elements[c].number;
							break;
						case ARRAY_UINT16_ELEMENT:
						case ARRAY_INT16_ELEMENT:
							size+=sizeof(xdl_int16)*m_elements[c].number;
							break;
						case ARRAY_UINT32_ELEMENT:
						case ARRAY_INT32_ELEMENT:
							size+=sizeof(xdl_int32)*m_elements[c].number;
							break;
						case ARRAY_UINT64_ELEMENT:
						case ARRAY_INT64_ELEMENT:
							size+=sizeof(xdl_int64)*m_elements[c].number;
							break;
						case ARRAY_FLOAT_ELEMENT:
							size+=sizeof(xdl_float)*m_elements[c].number;;
							break;
						case ARRAY_DOUBLE_ELEMENT:
							size+=sizeof(xdl_double)*m_elements[c].number;;
							break;
						default:
							;
					}
				}
				return size;
			}

			/// Clears the descriptor.
			/**
				Every element will be removed so that the getSize() function will return 0.
			*/
			void clear() {
				m_elements.clear();
			}

			xdl_bool operator==(XdevLArrayDeclaration& ad) {
				for(size_t c = 0; c < m_elements.size(); ++c) {
					if(m_elements[c].element != ad.m_elements[c].element)
						return xdl_false;
					if(m_elements[c].number != ad.m_elements[c].number)
						return xdl_false;
				}
				return xdl_true;
			}

			/// The overloaded '!=' compare operator.
			xdl_bool operator!=(XdevLArrayDeclaration& ad) {
				return !(ad == *this);
			}

			XdevLArrayItem* begin() {
				return &m_elements[0];
			}

			XdevLArrayItem* end() {
				return &m_elements[m_elements.size()];
			}

		protected:
			std::vector<XdevLArrayItem>	m_elements;
	};

	class XdevLScopeEndIncrease {
		public:
			XdevLScopeEndIncrease(xdl_uint64& value, xdl_uint size, xdl_uint max, xdl_uint min = 0) : m_size(size), m_value(&value), m_min(min), m_max(max) {
			}
			~XdevLScopeEndIncrease() {
				(*m_value) += m_size;
				if(*m_value > m_max) {
					(*m_value) = 0;
				}
			}
			const xdl_uint& 	m_size;
			xdl_uint64* m_value;
			xdl_uint64	m_min;
			xdl_uint64	m_max;
	};


	//
	// ADLER32 implementation
	//
	xdl_uint32 adler32(const xdl_char* buf, size_t buflength);


	/*
	 * If you do not have the ISO standard stdint.h header file, then you
	 * must typdef the following:
	 *    name              meaning
	 *  uint32_t         unsigned 32 bit integer
	 *  uint8_t          unsigned 8 bit integer (i.e., unsigned char)
	 *  int_least16_t    integer of >= 16 bits
	 *
	 */

	class SHA {
		public:
			enum {SHA1HashSize = 20};
			xdl_int reset();
			xdl_int getResult(const uint8_t* data, xdl_uint length, uint8_t* digit);
		protected:
			enum SHA1Enums {
			  shaSuccess = 0,
			  shaNull,            /* Null pointer parameter */
			  shaInputTooLong,    /* input data too long */
			  shaStateError       /* called Input after Result */
			};

		private:
			xdl_int setInput(const uint8_t* data, xdl_uint length);
			void processMessageBlock();
			void padMessage();
		private:
			uint32_t Intermediate_Hash[SHA1HashSize/4]; /* Message Digest  */

			uint32_t Length_Low;            /* Message length in bits      */
			uint32_t Length_High;           /* Message length in bits      */

			int_least16_t Message_Block_Index;
			uint8_t Message_Block[64];      /* 512-bit message blocks      */

			xdl_int Computed;               /* Is the digest computed?         */
			xdl_int Corrupted;             /* Is the message digest corrupted? */
	};

	std::ifstream::pos_type getSizeOfFile(const xdl::XdevLFileName& file);


	TiXmlElement* TiXmlFindElement(TiXmlDocument* doc, const xdl_char* elements);


}

#endif
