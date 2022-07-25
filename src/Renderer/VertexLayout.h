#pragma once
#include "VK_HEADER.h"

namespace LearnVulkan {
	class VertexLayout {
	public:
		struct VertexInputDescription {
			std::vector<VkVertexInputBindingDescription> bindings;
			std::vector<VkVertexInputAttributeDescription> attributes;

			VkPipelineVertexInputStateCreateFlags flags = 0;
		};

		VertexInputDescription description;

		VertexLayout();
		~VertexLayout();

		VertexLayout& begin();
		VertexLayout& add(VkFormat format, uint32_t size);
		VertexLayout& add_binding();
		void end();
		const uint32_t& getConstantSize();
	private:
		uint32_t layout_offset = 0;
		uint32_t constant_offset = 0;
		uint32_t location = 0;
	};
}
