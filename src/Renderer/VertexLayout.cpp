#include "VertexLayout.h"

namespace LearnVulkan {
	VertexLayout::VertexLayout() {}

	VertexLayout::~VertexLayout() {}

	VertexLayout& VertexLayout::begin()
	{
		location = 0;
		offset = 0;

		return *this;
	}

	VertexLayout& VertexLayout::add(VkFormat format, uint32_t size)
	{
		VkVertexInputAttributeDescription attribute = {};
		attribute.binding = 0;
		attribute.location = location;
		attribute.format = format;
		attribute.offset = offset;

		description.attributes.push_back(attribute);

		location++;
		offset += size;
		return *this;
	}

	void VertexLayout::end()
	{
		VkVertexInputBindingDescription mainBinding = {};
		mainBinding.binding = 0;
		mainBinding.stride = offset;
		mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		description.bindings.push_back(mainBinding);
	}
}
