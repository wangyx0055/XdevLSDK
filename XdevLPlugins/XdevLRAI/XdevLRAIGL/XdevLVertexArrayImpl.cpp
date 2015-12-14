#include "GL/glew.h"
#include <XdevLError.h>
#include "XdevLVertexArrayImpl.h"
#include "XdevLVertexBufferImpl.h"
#include "XdevLIndexBufferImpl.h"
#include "XdevLOpenGLUtils.h"
#include <iostream>

namespace xdl {

	XdevLVertexArrayImpl::XdevLVertexArrayImpl() : 	m_id(-1),
		m_vd(nullptr),
		m_indexBuffer(nullptr),
		m_activated(xdl_false) {
	}

	XdevLVertexArrayImpl::~XdevLVertexArrayImpl() {
		if(m_id != -1) {
			glDeleteVertexArrays(1, &m_id);
			m_id = -1;
			m_vd = nullptr;
		}
	}

	xdl_int XdevLVertexArrayImpl::init() {
		glGenVertexArrays(1, &m_id);
		if(m_id == -1) {
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	void  XdevLVertexArrayImpl::activate() {
		assert((m_id != -1) && "XdevLVertexArrayImpl::activate: Array not initialized.");

		glBindVertexArray(m_id);
		m_activated = xdl_true;
	}

	void  XdevLVertexArrayImpl::deactivate() {
		assert(m_activated && "XdevLVertexArrayImpl::activate: Not activated.");

		glBindVertexArray(0);
		m_activated = xdl_false;
	}

	xdl_int XdevLVertexArrayImpl::setVertexStreamBuffer(xdl_uint shaderAttribute, xdl_uint numberOfComponents, XdevLBufferElementTypes itemSizeType, XdevLVertexBuffer* vertexBuffer) {
		assert(m_activated && "XdevLVertexArrayImpl::activate: Not activated.");
		assert(vertexBuffer && "XdevLVertexArrayImpl::activate: No valid Vertex Buffer specified.");

		if(m_vd == NULL) {
			m_vd = new XdevLVertexDeclaration();
		}

		m_vd->add(numberOfComponents, itemSizeType, shaderAttribute);

		// Bind the Vertex Buffer to the array object.
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->id());

		glEnableVertexAttribArray(shaderAttribute);
		glVertexAttribPointer(shaderAttribute, numberOfComponents, itemSizeType, GL_FALSE, 0, (void*)(0));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		m_vertexBufferList.push_back(vertexBuffer);
		return ERR_OK;
	}

	xdl_int XdevLVertexArrayImpl::setIndexBuffer(XdevLIndexBuffer* indexBuffer) {
		assert(m_activated && "XdevLVertexArrayImpl::activate: Not activated.");
		assert(indexBuffer && "XdevLVertexArrayImpl::activate: No valid Index Buffer specified.");

		m_indexBuffer = indexBuffer;

		// Bind the Index Buffer to the array object.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->id());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return ERR_OK;
	}

	xdl_int XdevLVertexArrayImpl::init(xdl_uint8 numberOfStreamBuffers,
	                                   xdl_uint8* srcOfSreamBuffers[],
	                                   xdl_uint numberOfVertex,
	                                   XdevLVertexDeclaration* vd) {
		m_vd = vd;

		glGenVertexArrays(1, &m_id);
		glBindVertexArray(m_id);

		m_vertexBufferList.reserve(numberOfStreamBuffers);
		m_vertexBufferList.resize(numberOfStreamBuffers);

		for(xdl_uint a = 0; a < numberOfStreamBuffers; a++) {
			XdevLVertexBuffer* vb = new XdevLVertexBufferImpl();
			m_vertexBufferList[a] = vb;
		}

		for(xdl_uint idx = 0; idx < numberOfStreamBuffers; idx++) {

			GLuint shaderAttribute 	= m_vd->get(idx)->shaderAttribute;

			XdevLVertexDeclaration* vdecl = new XdevLVertexDeclaration();
			vdecl->add(m_vd->get(idx)->numberOfComponents, m_vd->get(idx)->elementType, m_vd->get(idx)->shaderAttribute);

			m_vertexBufferList[idx]->init(srcOfSreamBuffers[idx],  m_vd->vertexSize()*numberOfVertex);
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferList[idx]->id());

			glEnableVertexAttribArray(shaderAttribute);
			glVertexAttribPointer(shaderAttribute, m_vd->get(idx)->numberOfComponents, m_vd->get(idx)->elementType, GL_FALSE, 0, (void*)(0));

			glBindBuffer(GL_ARRAY_BUFFER, 0);

		}

		// Unbind Array Buffer object.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return ERR_OK;
	}

	xdl_int XdevLVertexArrayImpl::init(xdl_uint8* src, xdl_uint numberOfVertex, XdevLVertexDeclaration* vd) {
		m_vd = vd;

		// Create array object.
		glGenVertexArrays(1, &m_id);
		glBindVertexArray(m_id);

		// Create vertex buffer object.
		m_vertexBufferList.reserve(1);
		m_vertexBufferList.resize(1);

		XdevLVertexBuffer* vb = new XdevLVertexBufferImpl();
		vb->init(src,  vd->vertexSize()*numberOfVertex);
		m_vertexBufferList.push_back(vb);
		vb->activate();

		xdl_uint64 pos = 0;
		for(xdl_uint idx = 0; idx < m_vd->getNumber(); idx++) {

			GLuint shader_attrib = m_vd->get(idx)->shaderAttribute;
			glEnableVertexAttribArray(shader_attrib);
			glVertexAttribPointer(shader_attrib,
			                      m_vd->get(idx)->numberOfComponents,
			                      m_vd->get(idx)->elementType,
			                      GL_FALSE,
			                      m_vd->vertexStride(),
			                      BUFFER_OFFSET(pos));

			pos += m_vd->get(idx)->numberOfComponents*m_vd->get(idx)->elementTypeSizeInBytes;
		}

		vb->deactivate();
		glBindVertexArray(0);
		return ERR_OK;
	}

	xdl_int  XdevLVertexArrayImpl::init(XdevLVertexBuffer* vertexBuffer, XdevLVertexDeclaration* vd) {
		m_vd = vd;

		// Create array object.
		glGenVertexArrays(1, &m_id);
		glBindVertexArray(m_id);

		// Create vertex buffer object.
		m_vertexBufferList.reserve(1);
		m_vertexBufferList.resize(1);

		m_vertexBufferList.push_back(vertexBuffer);
		vertexBuffer->activate();

		xdl_uint64 pos = 0;
		for(xdl_uint idx = 0; idx < m_vd->getNumber(); idx++) {

			GLuint shader_attrib = m_vd->get(idx)->shaderAttribute;
			glEnableVertexAttribArray(shader_attrib);
			glVertexAttribPointer(shader_attrib,
			                      m_vd->get(idx)->numberOfComponents,
			                      m_vd->get(idx)->elementType,
			                      GL_FALSE,
			                      m_vd->vertexStride(),
			                      BUFFER_OFFSET(pos));

			pos += m_vd->get(idx)->numberOfComponents*m_vd->get(idx)->elementTypeSizeInBytes;
		}

		glBindVertexArray(0);

		return ERR_OK;
	}

	xdl_int XdevLVertexArrayImpl::init(xdl_uint32 numberIndices,
	                                   xdl_uint8* srcOfIndices,
	                                   xdl_uint8 numberOfStreamBuffers,
	                                   xdl_uint8* srcOfSreamBuffers[],
	                                   xdl_uint numberOfVertex,
	                                   XdevLVertexDeclaration* vd) {



		m_vd = vd;

		glGenVertexArrays(1, &m_id);
		glBindVertexArray(m_id);

		m_indexBuffer = new XdevLIndexBufferImpl();
		m_indexBuffer->init();
		m_indexBuffer->activate();

		m_indexBuffer->upload(srcOfIndices, sizeof(xdl_uint)*numberIndices);



		m_vertexBufferList.reserve(numberOfStreamBuffers);
		m_vertexBufferList.resize(numberOfStreamBuffers);

		for(xdl_uint a = 0; a < numberOfStreamBuffers; a++) {
			XdevLVertexBuffer* vb = new XdevLVertexBufferImpl();
			m_vertexBufferList[a] = vb;
		}


		for(xdl_uint idx = 0; idx < numberOfStreamBuffers; idx++) {

			GLuint shaderAttribute 	= m_vd->get(idx)->shaderAttribute;

			XdevLVertexDeclaration* vdecl = new XdevLVertexDeclaration();
			vdecl->add(m_vd->get(idx)->numberOfComponents, m_vd->get(idx)->elementType, shaderAttribute);

			m_vertexBufferList[idx]->init(srcOfSreamBuffers[idx], vdecl->vertexSize()*numberOfVertex);
			m_vertexBufferList[idx]->activate();

			glEnableVertexAttribArray(shaderAttribute);
			glVertexAttribPointer(shaderAttribute, m_vd->get(idx)->numberOfComponents, m_vd->get(idx)->elementType, GL_FALSE, 0, (void*)(0));



		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return ERR_OK;
	}


	XdevLVertexDeclaration* XdevLVertexArrayImpl::getVertexDeclaration() {
		return m_vd;
	}

	xdl_uint XdevLVertexArrayImpl::id() {
		return m_id;
	}

	XdevLVertexBuffer* XdevLVertexArrayImpl::getVertexBuffer(xdl_uint indexNumber) {

		return m_vertexBufferList[indexNumber];
	}

	XdevLIndexBuffer* XdevLVertexArrayImpl::getIndexBuffer() {
		return m_indexBuffer;
	}

}
