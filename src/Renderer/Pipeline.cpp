#include "Pipeline.h"

namespace LearnVulkan {
	VkPipeline PipelineBuilder::build_pipeline(VkDevice device, VkRenderPass pass)
	{
		// make viewport state from our stored viewport and scissor.
		// at the moment we wont support multiple viewports or scissors
		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.pNext = nullptr;

		viewportState.viewportCount = 1;
		viewportState.pViewports = &_viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &_scissor;

		// setup dummy color blending. We arent using transparent objects yet
		// the blending is just "no blend", but we do write to the color attachment
		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.pNext = nullptr;

		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &_colorBlendAttachment;

		// build the actual pipeline
		// we now use all of the info structs we have been writing into into this one to create the pipeline
		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.pNext = nullptr;

		pipelineInfo.stageCount = _shaderStages.size();
		pipelineInfo.pStages = _shaderStages.data();
		pipelineInfo.pVertexInputState = &_vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &_inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &_rasterizer;
		pipelineInfo.pMultisampleState = &_multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.layout = _pipelineLayout;
		pipelineInfo.renderPass = pass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.pDepthStencilState = &_depthStencil;

		// its easy to error out on create graphics pipeline, so we handle it a bit better than the common VK_CHECK case
		VkPipeline newPipeline;
		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &newPipeline) != VK_SUCCESS) {
			Logger::console->error("failed to create pipline");
			return VK_NULL_HANDLE; // failed to create graphics pipeline
		}
		else
		{
			return newPipeline;
		}
	}

	Pipeline::Pipeline(Device* device, VkRenderPass* renderPass, Descriptor* descriptor) : device(device), _renderPass(renderPass), descriptor(descriptor) {
		add_constant(sizeof(MeshPushConstants));
	}

	Pipeline::~Pipeline() {}

	Pipeline& Pipeline::add_constant(uint32_t size)
	{
		// setup push constants
		VkPushConstantRange push_constant;
		// this push constant range starts at the beginning
		push_constant.offset = constant_offset;
		// this push constant range takes up the size of a MeshPushConstants struct
		push_constant.size = size;
		// this push constant range is accessible only in the vertex shader
		push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		constants.push_back(push_constant);

		constant_offset += size;
		return *this;
	}

	VkShaderModule Pipeline::load_shader_module(const char* filePath)
	{
		// open the file. With cursor at the end
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw Error(fmt::format("Failed to open file: {}", filePath));
		}

		// find what the size of the file is by looking up the location of the cursor
		// because the cursor is at the end, it gives the size directly in bytes
		size_t fileSize = (size_t)file.tellg();

		// spirv expects the buffer to be on uint32, so make sure to reserve a int vector big enough for the entire file
		std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

		// put file cursor at beggining
		file.seekg(0);

		// load the entire file into the buffer
		file.read((char*)buffer.data(), fileSize);

		// now that the file is loaded into the buffer, we can close it
		file.close();

		// create a new shader module, using the buffer we loaded
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.pNext = nullptr;

		// codeSize has to be in bytes, so multply the ints in the buffer by size of int to know the real size of the buffer
		createInfo.codeSize = buffer.size() * sizeof(uint32_t);
		createInfo.pCode = buffer.data();

		// check that the creation goes well.
		VkShaderModule shaderModule;
		if (vkCreateShaderModule(device->_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			throw Error(fmt::format("Failed to create shader module: {}", filePath));
		}

		return shaderModule;
	}

	Pipeline& Pipeline::init_layout(std::vector<VkDescriptorSetLayout>& descriptorLayouts)
	{
		pipelineBuilder = {};

		VkPipelineLayout pipelineLayout;

		std::vector<VkDescriptorSetLayout> combined_layouts = descriptor->_layouts;
		// combine custom layouts and default descriptor layouts
		for (auto iter = descriptorLayouts.begin(); iter != descriptorLayouts.end(); ++iter) {
			combined_layouts.push_back(*iter);
		}

		// build the pipeline layout that controls the inputs/outputs of the shader
		// add default push_constants, for instance: MVP
		VkPipelineLayoutCreateInfo pipeline_layout_info = vkinit::pipeline_layout_create_info(constants, combined_layouts);

		VK_CHECK(vkCreatePipelineLayout(device->_device, &pipeline_layout_info, nullptr, &pipelineLayout));

		// use the layout we created
		pipelineBuilder._pipelineLayout = pipelineLayout;

		// input assembly is the configuration for drawing triangle lists, strips, or individual points.
		// we are just going to draw triangle list
		pipelineBuilder._inputAssembly = vkinit::input_assembly_create_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

		// build viewport and scissor from the swapchain extents
		pipelineBuilder._viewport.x = 0.0f;
		pipelineBuilder._viewport.y = 0.0f;
		pipelineBuilder._viewport.width = (float)device->_windowExtent.width;
		pipelineBuilder._viewport.height = (float)device->_windowExtent.height;
		pipelineBuilder._viewport.minDepth = 0.0f;
		pipelineBuilder._viewport.maxDepth = 1.0f;

		pipelineBuilder._scissor.offset = { 0, 0 };
		pipelineBuilder._scissor.extent = device->_windowExtent;

		// configure the rasterizer to draw filled triangles
		pipelineBuilder._rasterizer = vkinit::rasterization_state_create_info(VK_POLYGON_MODE_FILL);

		// we dont use multisampling, so just run the default one
		pipelineBuilder._multisampling = vkinit::multisampling_state_create_info();

		// a single blend attachment with no blending and writing to RGBA
		pipelineBuilder._colorBlendAttachment = vkinit::color_blend_attachment_state();

		// default depthtesting
		pipelineBuilder._depthStencil = vkinit::depth_stencil_create_info(true, true, VK_COMPARE_OP_LESS_OR_EQUAL);

		layouts.push_back(pipelineLayout);

		return *this;
	}

	Pipeline& Pipeline::init_vertex_layout(VertexLayout& layout)
	{
		// vertex input controls how to read vertices from vertex buffers.
		pipelineBuilder._vertexInputInfo = vkinit::vertex_input_state_create_info(layout);

		return *this;
	}

	Pipeline& Pipeline::add_pipeline(std::string shaderName)
	{
		try {
			VkShaderModule vertexShader = load_shader_module(fmt::format("{0}\\{1}_vert.spv", SHADER_FOLDER, shaderName).c_str());
			VkShaderModule fragShader = load_shader_module(fmt::format("{0}\\{1}_frag.spv", SHADER_FOLDER, shaderName).c_str());

			// add the other shaders
			pipelineBuilder._shaderStages.push_back(
				vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, vertexShader));

			pipelineBuilder._shaderStages.push_back(
				vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, fragShader));

			// build pipeline
			pipelines.push_back(pipelineBuilder.build_pipeline(device->_device, *_renderPass));

			// clear the shader stages for the builder
			pipelineBuilder._shaderStages.clear();

			vkDestroyShaderModule(device->_device, vertexShader, nullptr);
			vkDestroyShaderModule(device->_device, fragShader, nullptr);
		}
		catch (Error error) {
			Logger::console->error(error.desc);
		}

		return *this;
	}

	void Pipeline::Destroy()
	{
		for (auto iter = pipelines.begin(); iter != pipelines.end(); ++iter) {
			vkDestroyPipeline(device->_device, *iter, nullptr);
		}

		for (auto iter = layouts.begin(); iter != layouts.end(); ++iter) {
			vkDestroyPipelineLayout(device->_device, *iter, nullptr);
		}
	}
}
