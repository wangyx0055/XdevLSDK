#ifndef XDEVL_VERTEX_BUFFER_H
#define XDEVL_VERTEX_BUFFER_H

#include <XdevLRAI/XdevLVertexDeclaration.h>
#include <XdevLRAI/XdevLStreamBuffer.h>

namespace xdl {

	class XdevLVertexDeclaration;

	class XdevLVertexBuffer : public XdevLStreamBuffer {
		public:
			virtual ~XdevLVertexBuffer() {}
	};

}

#endif
