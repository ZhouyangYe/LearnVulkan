#include "MeshBuffer.h"

namespace LearnVulkan {
	VertexBuffer::VertexBuffer() {}

	VertexBuffer::~VertexBuffer() {}

	void VertexBuffer::bind_pipeline(Pipeline* pipeline)
	{
		this->pipeline = pipeline;
	}

	void VertexBuffer::setSelectedPipeline(uint32_t index)
	{
		selectedIndex = index;
	}

	void VertexBuffer::upload_mesh(Device& device, const void* vertices, uint64_t size, uint32_t num)
	{
		this->device = &device;
		vertice_num = num;

		//allocate vertex buffer
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		//this is the total size, in bytes, of the buffer we are allocating
		bufferInfo.size = size;
		//this buffer is going to be used as a Vertex Buffer
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;


		//let the VMA library know that this data should be writeable by CPU, but also readable by GPU
		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

		//allocate the buffer
		VK_CHECK(vmaCreateBuffer(device._allocator, &bufferInfo, &vmaallocInfo,
			&_buffer,
			&_allocation,
			nullptr));

		//copy vertex data
		void* data;
		vmaMapMemory(device._allocator, _allocation, &data);

		memcpy(data, vertices, size);

		vmaUnmapMemory(device._allocator, _allocation);
	}

	void VertexBuffer::Destroy()
	{
		vmaDestroyBuffer(device->_allocator, _buffer, _allocation);
	}
}
