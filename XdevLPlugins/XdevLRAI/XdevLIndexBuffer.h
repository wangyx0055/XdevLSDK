#ifndef XDEVL_INDEX_BUFFER_H
#define XDEVL_INDEX_BUFFER_H

#include <XdevLRAI/XdevLVertexDeclaration.h>
#include <XdevLRAI/XdevLStreamBuffer.h>

namespace xdl {

	class XdevLIndexBuffer : public XdevLStreamBuffer {
		public:
			virtual ~XdevLIndexBuffer() {}

			/// Initialize an empty buffer.
			using XdevLStreamBuffer::init;

			/// Initialize the vertex buffer.
			virtual xdl_int	init(XdevLBufferElementTypes indexBufferType, xdl_uint8* src, xdl_uint numberOfIndices) = 0;

			/// Returns the type of element used in the buffer.
			virtual XdevLBufferElementTypes	getElementType() = 0;

	};

}

#endif
