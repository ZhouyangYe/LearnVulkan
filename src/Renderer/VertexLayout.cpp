#include "VertexLayout.h"

namespace LearnVulkan {
	VertexLayout::VertexLayout() {}

	VertexLayout::~VertexLayout() {}

	const uint32_t& VertexLayout::getConstantSize()
	{
		return constant_offset;
	}

	VertexLayout& VertexLayout::begin()
	{
		constant_offset = 0;

		return *this;
	}

	VertexLayout& VertexLayout::add(VkFormat format, uint32_t size)
	{
		VkVertexInputAttributeDescription attribute = {};
		attribute.binding = 0;
		attribute.location = location;
		attribute.format = format;
		attribute.offset = layout_offset;

		description.attributes.push_back(attribute);

		location++;
		layout_offset += size;
		return *this;
	}

	VertexLayout& VertexLayout::add_binding()
	{
		// add bindings
		VkVertexInputBindingDescription binding = {};
		binding.binding = 0;
		binding.stride = layout_offset;
		binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		description.bindings.push_back(binding);

		layout_offset = 0;
		location = 0;
		return *this;
	}


	void VertexLayout::end()
	{

	}
}
