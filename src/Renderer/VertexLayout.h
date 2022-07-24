#pragma once
#include "VK_HEADER.h"

namespace LearnVulkan {
	class VertexLayout {
	public:
		struct VertexInputDescription {
			std::vector<VkVertexInputBindingDescription> bindings;
			std::vector<VkVertexInputAttributeDescription> attributes;
			std::vector<VkPushConstantRange> constants;
			std::vector<const void*> constants_data;

			VkPipelineVertexInputStateCreateFlags flags = 0;
		};

		VertexInputDescription description;

		VertexLayout();
		~VertexLayout();

		VertexLayout& begin();
		VertexLayout& add(VkFormat format, uint32_t size);
		VertexLayout& add_constant(uint32_t size, const void* data);
		void end();
		const uint32_t& getConstantSize();
	private:
		uint32_t layout_offset = 0;
		uint32_t constant_offset = 0;
		uint32_t location = 0;
	};
}
