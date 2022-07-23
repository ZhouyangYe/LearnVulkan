#pragma once
#include "VK_HEADER.h"

namespace LearnVulkan {
	class VertexBuffer {
	public:
		uint32_t vertice_num = 0;
		VkBuffer _buffer;
		VmaAllocation _allocation;

		VertexBuffer();
		~VertexBuffer();
	};
}
