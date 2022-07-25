#pragma once
#include "VK_HEADER.h"

namespace LearnVulkan {
	class VertexBuffer {
	public:
		VkBuffer _buffer;
		VmaAllocation _allocation;

		VertexBuffer();
		~VertexBuffer();
	};
}
