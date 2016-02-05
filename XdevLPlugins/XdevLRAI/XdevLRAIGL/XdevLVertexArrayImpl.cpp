#include "GL/glew.h"
#include <XdevLError.h>
#include "XdevLVertexArrayImpl.h"
#include "XdevLVertexBufferImpl.h"
#include "XdevLIndexBufferImpl.h"
#include "XdevLOpenGLUtils.h"
#include <iostream>

namespace xdl {

	XdevLVertexArrayImpl::XdevLVertexArrayImpl() :
		m_id(0),
		m_vd(nullptr),
		m_indexBuffer(nullptr),
		m_activated(xdl_false) {
	}

	XdevLVertexArrayImpl::~XdevLVertexArrayImpl() {
		if(0 != m_id) {
			glDeleteVertexArrays(1, &m_id);
		}
	}

	xdl_int XdevLVertexArrayImpl::init() {
		glGenVertexArrays(1, &m_id);
		if(0 == m_id) {
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	xdl_int XdevLVertexArrayImpl::activate() {
		assert(!m_activated && "XdevLVertexArrayImpl::activate: Array initialized already.");

		glBindVertexArray(m_id);
		if(!glIsVertexArray(m_id)) {
			return ERR_ERROR;
		}

		m_activated = xdl_true;

		return ERR_OK;
	}

	xdl_int  XdevLVertexArrayImpl::deactivate() {
		assert(m_activated && "XdevLVertexArrayImpl::activate: Not activated.");

		glBindVertexArray(0);
		if(!glIsVertexArray(m_id)) {
			return ERR_ERROR;
		}

		m_activated = xdl_false;
		return ERR_OK;
	}

	xdl_int XdevLVertexArrayImpl::setVertexStreamBuffer(xdl_uint shaderAttribute, xdl_uint numberOfComponents, XdevLBufferElementTypes itemSizeType, IPXdevLVertexBuffer vertexBuffer) {
		assert(m_activated && "XdevLVertexArrayImpl::activate: Not activated.");
		assert(vertexBuffer && "XdevLVertexArrayImpl::activate: No valid Vertex Buffer specified.");

		if(m_vd == NULL) {
			m_vd = std::make_shared<XdevLVertexDeclaration>();
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

	xdl_int XdevLVertexArrayImpl::setIndexBuffer(IPXdevLIndexBuffer indexBuffer) {
		assert(m_activated && "XdevLVertexArrayImpl::activate: Not activated.");
		assert(indexBuffer && "XdevLVertexArrayImpl::activate: No valid Index Buffer specified.");

		m_indexBuffer = indexBuffer;

		// Bind the Index Buffer to the array object.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->id());

		return ERR_OK;
	}

	xdl_int XdevLVertexArrayImpl::init(xdl_uint8 numberOfStreamBuffers,
	                                   xdl_uint8* srcOfSreamBuffers[],
	                                   xdl_uint numberOfVertex,
	                                   IPXdevLVertexDeclaration vd) {
		m_vd = vd;

		glGenVertexArrays(1, &m_id);
		glBindVertexArray(m_id);

		m_vertexBufferList.reserve(numberOfStreamBuffers);
		m_vertexBufferList.resize(numberOfStreamBuffers);

		for(xdl_uint a = 0; a < numberOfStreamBuffers; a++) {
			IPXdevLVertexBuffer vb = std::shared_ptr<XdevLVertexBufferImpl>(new XdevLVertexBufferImpl());
			m_vertexBufferList[a] = vb;
		}

		for(xdl_uint idx = 0; idx < numberOfStreamBuffers; idx++) {

			GLuint shaderAttribute 	= m_vd->get(idx)->shaderAttribute;

			m_vertexBufferList[idx]->init(srcOfSreamBuffers[idx],  m_vd->vertexSize()*numberOfVertex);

			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferList[idx]->id());
			glEnableVertexAttribArray(shaderAttribute);
			glVertexAttribPointer(shaderAttribute, m_vd->get(idx)->numberOfComponents, m_vd->get(idx)->elementType, GL_FALSE, 0, (void*)(0));
		}

		glBindVertexArray(0);

		return ERR_OK;
	}

	xdl_int XdevLVertexArrayImpl::init(xdl_uint8* src, xdl_uint numberOfVertex, IPXdevLVertexDeclaration vd) {
		m_vd = vd;

		// Create array object.
		glGenVertexArrays(1, &m_id);
		glBindVertexArray(m_id);

		// Create vertex buffer object.
		m_vertexBufferList.reserve(1);
		m_vertexBufferList.resize(1);

		IPXdevLVertexBuffer vb = std::shared_ptr<XdevLVertexBufferImpl>(new XdevLVertexBufferImpl());
		vb->init(src,  vd->vertexSize()*numberOfVertex);
		m_vertexBufferList.push_back(vb);
		glBindBuffer(GL_ARRAY_BUFFER, vb->id());

		xdl_uint64 pos = 0;
		for(xdl_uint idx = 0; idx < m_vd->getNumber(); idx++) {

			GLuint shaderAttribute = m_vd->get(idx)->shaderAttribute;
			glEnableVertexAttribArray(shaderAttribute);
			glVertexAttribPointer(shaderAttribute,
			                      m_vd->get(idx)->numberOfComponents,
			                      m_vd->get(idx)->elementType,
			                      GL_FALSE,
			                      m_vd->vertexStride(),
			                      BUFFER_OFFSET(pos));

			pos += m_vd->get(idx)->numberOfComponents*m_vd->get(idx)->elementTypeSizeInBytes;
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return ERR_OK;
	}

	xdl_int  XdevLVertexArrayImpl::init(IPXdevLVertexBuffer vertexBuffer, IPXdevLVertexDeclaration vd) {
		m_vd = vd;

		// Create array object.
		glGenVertexArrays(1, &m_id);
		glBindVertexArray(m_id);

		// Create vertex buffer object.
		m_vertexBufferList.reserve(1);
		m_vertexBufferList.resize(1);

		m_vertexBufferList.push_back(vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->id());

		xdl_uint64 pos = 0;
		for(xdl_uint idx = 0; idx < m_vd->getNumber(); idx++) {

			GLuint shaderAttribute = m_vd->get(idx)->shaderAttribute;
			glEnableVertexAttribArray(shaderAttribute);
			glVertexAttribPointer(shaderAttribute,
			                      m_vd->get(idx)->numberOfComponents,
			                      m_vd->get(idx)->elementType,
			                      GL_FALSE,
			                      m_vd->vertexStride(),
			                      BUFFER_OFFSET(pos));

			pos += m_vd->get(idx)->numberOfComponents*m_vd->get(idx)->elementTypeSizeInBytes;
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return ERR_OK;
	}

	xdl_int XdevLVertexArrayImpl::init(IPXdevLVertexBuffer vertexBuffer, IPXdevLIndexBuffer indexBuffer, IPXdevLVertexDeclaration vd) {
		m_vd = vd;
		m_indexBuffer = indexBuffer;

		// Create array object.
		glGenVertexArrays(1, &m_id);
		glBindVertexArray(m_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->id());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->id());

		// Create vertex buffer object.
		m_vertexBufferList.push_back(vertexBuffer);

		xdl_uint64 pos = 0;
		for(xdl_uint idx = 0; idx < m_vd->getNumber(); idx++) {

			GLuint shaderAttribute = m_vd->get(idx)->shaderAttribute;
			glEnableVertexAttribArray(shaderAttribute);
			glVertexAttribPointer(shaderAttribute,
			                      m_vd->get(idx)->numberOfComponents,
			                      m_vd->get(idx)->elementType,
			                      GL_FALSE,
			                      m_vd->vertexStride(),
			                      BUFFER_OFFSET(pos));

			pos += m_vd->get(idx)->numberOfComponents*m_vd->get(idx)->elementTypeSizeInBytes;
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return ERR_OK;
	}

	xdl_int XdevLVertexArrayImpl::init(xdl_uint32 numberIndices,
	                                   xdl_uint8* srcOfIndices,
	                                   xdl_uint8 numberOfStreamBuffers,
	                                   xdl_uint8* srcOfSreamBuffers[],
	                                   xdl_uint numberOfVertex,
	                                   IPXdevLVertexDeclaration vd) {



		m_vd = vd;

		m_indexBuffer = std::shared_ptr<XdevLIndexBufferImpl>(new XdevLIndexBufferImpl());
		m_indexBuffer->init();

		m_indexBuffer->lock();
		m_indexBuffer->upload(srcOfIndices, sizeof(xdl_uint)*numberIndices);
		m_indexBuffer->unlock();

		glGenVertexArrays(1, &m_id);
		glBindVertexArray(m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer->id());

		m_vertexBufferList.reserve(numberOfStreamBuffers);
		m_vertexBufferList.resize(numberOfStreamBuffers);

		for(xdl_uint a = 0; a < numberOfStreamBuffers; a++) {
			IPXdevLVertexBuffer vb = std::shared_ptr<XdevLVertexBufferImpl>(new XdevLVertexBufferImpl());
			m_vertexBufferList[a] = vb;
		}


		for(xdl_uint idx = 0; idx < numberOfStreamBuffers; idx++) {

			GLuint shaderAttribute 	= m_vd->get(idx)->shaderAttribute;

			XdevLVertexDeclaration* vdecl = new XdevLVertexDeclaration();
			vdecl->add(m_vd->get(idx)->numberOfComponents, m_vd->get(idx)->elementType, shaderAttribute);

			m_vertexBufferList[idx]->init(srcOfSreamBuffers[idx], vdecl->vertexSize()*numberOfVertex);
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferList[idx]->id());

			glEnableVertexAttribArray(shaderAttribute);
			glVertexAttribPointer(shaderAttribute, m_vd->get(idx)->numberOfComponents, m_vd->get(idx)->elementType, GL_FALSE, 0, (void*)(0));
		}

		//
		// Let's unbind all objects so that nothing can get messed up by other OpenGL code fragments.
		//
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return ERR_OK;
	}


	IPXdevLVertexDeclaration XdevLVertexArrayImpl::getVertexDeclarationRef() {
		return m_vd;
	}

	XdevLVertexDeclaration* XdevLVertexArrayImpl::getVertexDeclaration() {
		return m_vd.get();
	}

	xdl_uint XdevLVertexArrayImpl::id() {
		return m_id;
	}

	IPXdevLVertexBuffer XdevLVertexArrayImpl::getVertexBufferRef(xdl_uint indexNumber) {
		return m_vertexBufferList[indexNumber];
	}

	IPXdevLIndexBuffer XdevLVertexArrayImpl::getIndexBufferRef() {
		return m_indexBuffer;
	}

	XdevLVertexBuffer* XdevLVertexArrayImpl::getVertexBuffer(xdl_uint indexNumber) {
		return m_vertexBufferList[indexNumber].get();
	}

	XdevLIndexBuffer* XdevLVertexArrayImpl::getIndexBuffer() {
		return m_indexBuffer.get();
	}

}
