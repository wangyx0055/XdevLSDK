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

#include <XdevLTypes.h>
#include <XdevLError.h>
#include "XdevLVertexBufferVulkan.h"

#include <cstring>

namespace xdl {

	XdevLVertexBufferVulkan::XdevLVertexBufferVulkan() :
		m_locked(xdl_false),
		m_mapped(xdl_false),
		m_size(0) {
	
	}

	XdevLVertexBufferVulkan::~XdevLVertexBufferVulkan() {
	}

	xdl_int XdevLVertexBufferVulkan::init() {

		return ERR_OK;
	}

	xdl_int XdevLVertexBufferVulkan::init(xdl_uint8* src, xdl_uint size) {

		m_bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		m_bufferCreateInfo.pNext = nullptr;
		m_bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		m_bufferCreateInfo.size = size;
		m_bufferCreateInfo.queueFamilyIndexCount = 0;
		m_bufferCreateInfo.pQueueFamilyIndices = nullptr;
		m_bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		m_bufferCreateInfo.flags = 0;

		VkResult result = vkCreateBuffer(m_device, &m_bufferCreateInfo, nullptr, &m_buffer);
		if(VK_SUCCESS != result) {
			return ERR_ERROR;
		}


		vkGetBufferMemoryRequirements(m_device, m_buffer, &m_memReqs);

		VkMemoryAllocateInfo alloc_info = {};
		alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		alloc_info.pNext = nullptr;
		alloc_info.memoryTypeIndex = 0;
		alloc_info.allocationSize = m_memReqs.size;

		result = vkAllocateMemory(m_device, &alloc_info, nullptr, &(m_deviceMemory));
		if(VK_SUCCESS != result) {
			return ERR_ERROR;
		}

		xdl_uint8* pData;
		result = vkMapMemory(m_device, m_deviceMemory, 0, m_memReqs.size, 0, (void **)&pData);
		if(VK_SUCCESS != result) {
			return ERR_ERROR;
		}

		memcpy(pData, src, size);

		vkUnmapMemory(m_device, m_deviceMemory);
		m_size = size;
		return ERR_OK;
	}

	xdl_int XdevLVertexBufferVulkan::lock() {
		assert(!m_locked && "XdevLVertexBufferVulkan::lock: Was locked already.");
		m_locked = xdl_true;
		return ERR_ERROR;
	}

	xdl_int XdevLVertexBufferVulkan::unlock() {
		assert(m_locked && "XdevLVertexBufferVulkan::unlock: Was not locked.");
		m_locked = xdl_false;
		return ERR_ERROR;
	}

	xdl_uint8* XdevLVertexBufferVulkan::map(XdevLBufferAccessType bufferAccessType) {
		assert(m_locked && "XdevLVertexBufferVulkan::map: Was not locked.");
		assert(!m_mapped && "XdevLVertexBufferVulkan::map: Was mapped already.");

		xdl_uint8* pData;
		VkResult result = vkMapMemory(m_device, m_deviceMemory, 0, m_memReqs.size, 0, (void **)&pData);
		if(VK_SUCCESS != result) {
			return nullptr;
		}
		m_mapped = xdl_true;
		return pData;
	}

	xdl_int XdevLVertexBufferVulkan::unmap() {
		assert(m_mapped && "XdevLVertexBufferVulkan::map: Was not mapped.");

		vkUnmapMemory(m_device, m_deviceMemory);
		m_mapped = xdl_false;
		return ERR_OK;
	}

	xdl_int XdevLVertexBufferVulkan::upload(xdl_uint8* src, xdl_uint size) {
		if(m_memReqs.size > size) {
			return ERR_ERROR;
		}

		xdl_uint8* pData;
		VkResult result = vkMapMemory(m_device, m_deviceMemory, 0, m_memReqs.size, 0, (void **)&pData);
		if(VK_SUCCESS != result) {
			return ERR_ERROR;
		}

		memcpy(pData, src, size);

		vkUnmapMemory(m_device, m_deviceMemory);
		return ERR_ERROR;
	}

	xdl_uint XdevLVertexBufferVulkan::id() {
		return 0;
	}

	xdl_uint XdevLVertexBufferVulkan::getSize() {
		return 0;
	}

}

