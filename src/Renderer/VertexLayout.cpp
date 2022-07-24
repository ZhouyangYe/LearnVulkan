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
		location = 0;
		layout_offset = 0;
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

	VertexLayout& VertexLayout::add_constant(uint32_t size, const void* data)
	{
		//setup push constants
		VkPushConstantRange push_constant;
		//this push constant range starts at the beginning
		push_constant.offset = constant_offset;
		//this push constant range takes up the size of a MeshPushConstants struct
		push_constant.size = size;
		//this push constant range is accessible only in the vertex shader
		push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		description.constants.push_back(push_constant);
		description.constants_data.push_back(data);

		constant_offset += size;
		return *this;
	}

	void VertexLayout::end()
	{
		// add bindings
		VkVertexInputBindingDescription mainBinding = {};
		mainBinding.binding = 0;
		mainBinding.stride = layout_offset;
		mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		description.bindings.push_back(mainBinding);
	}
}
