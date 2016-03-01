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

#ifndef XDEVL_VERTEX_BUFFER_VULKAN_H
#define XDEVL_VERTEX_BUFFER_VULKAN_H

#include <vulkan/vulkan.h>
#include "XdevLVertexBufferVulkan.h"

namespace xdl {

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

		VKresult result = vkCreateBuffer(m_device, &m_bufferCreateInfo, nullptr, &m_buffer);
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

		vkUnmapMemory(info.device, info.vertex_buffer.mem);

		return ERR_OK;
	}

	xdl_int XdevLVertexBufferVulkan::lock() {

	}

	xdl_int XdevLVertexBufferVulkan::unlock() {

	}

	xdl_uint8* XdevLVertexBufferVulkan::map(XdevLBufferAccessType bufferAccessType) {

	}

	xdl_int XdevLVertexBufferVulkan::unmap() {

	}

	xdl_int XdevLVertexBufferVulkan::upload(xdl_uint8* src, xdl_uint size) {

	}

	xdl_uint XdevLVertexBufferVulkan::id() {

	}

	xdl_uint XdevLVertexBufferVulkan::getSize() {

	}
};

}

#endif
