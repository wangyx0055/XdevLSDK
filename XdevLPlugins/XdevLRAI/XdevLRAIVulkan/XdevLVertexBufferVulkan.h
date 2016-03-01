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

#include <XdevLTypes.h>
#include <XdevLRAI/XdevLVertexBuffer.h>

namespace xdl {

	class XdevLVertexBufferVulkan : public XdevLVertexBuffer {
		public:
			XdevLVertexBufferVulkan();
			virtual ~XdevLVertexBufferVulkan();
			xdl_int init() override;
			xdl_int init(xdl_uint8* src, xdl_uint size) override;
			xdl_int lock() override;
			xdl_int unlock() override;
			xdl_uint8* map(XdevLBufferAccessType bufferAccessType) override;
			xdl_int unmap() override;
			xdl_int upload(xdl_uint8* src, xdl_uint size) override;
			xdl_uint id() override;
			xdl_uint getSize() override;
		private:
			VkDevice m_device;
			VkBufferCreateInfo m_bufferCreateInfo;
			VkDescriptorBufferInfo m_descriptorBufferInfo;
			VkBuffer m_buffer;
			VkDeviceMemory m_deviceMemory;
			VkMemoryRequirements m_memReqs;
			xdl_bool m_locked;
			xdl_bool m_mapped;
			xdl_uint m_size;
	};

}

#endif
