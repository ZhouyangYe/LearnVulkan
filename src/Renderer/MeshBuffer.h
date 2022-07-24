#pragma once
#include "VK_HEADER.h"
#include "Pipeline.h"
#include "Device.h"

namespace LearnVulkan {
	class VertexBuffer {
	public:
		uint32_t vertice_num = 0;
		VkBuffer _buffer;
		VmaAllocation _allocation;
		Pipeline* pipeline;
		uint32_t selectedIndex = 0;

		VertexBuffer();
		~VertexBuffer();

		void setSelectedPipeline(uint32_t index);
		void bind_pipeline(Pipeline* pipeline);
		void upload_mesh(Device& device, const void* vertices, uint64_t size, uint32_t num);
		void Destroy();
	private:
		Device* device;
	};
}
