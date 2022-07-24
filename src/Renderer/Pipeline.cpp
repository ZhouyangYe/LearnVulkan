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

		// its easy to error out on create graphics pipeline, so we handle it a bit better than the common VK_CHECK case
		VkPipeline newPipeline;
		if (vkCreateGraphicsPipelines(
			device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &newPipeline) != VK_SUCCESS) {
			std::cout << "failed to create pipline\n";
			return VK_NULL_HANDLE; // failed to create graphics pipeline
		}
		else
		{
			return newPipeline;
		}
	}

	Pipeline::Pipeline() {}

	Pipeline::~Pipeline() {}

	VkShaderModule Pipeline::load_shader_module(const char* filePath)
	{
		// open the file. With cursor at the end
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw ShaderError(fmt::format("Failed to open file: ", filePath));
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
			throw ShaderError(fmt::format("Failed to create shader module: ", filePath));
		}

		return shaderModule;
	}

	void Pipeline::init_pipeline(Device* device, VkRenderPass* renderPass, VertexLayout& layout)
	{
		this->layout = &layout;
		this->device = device;
		_renderPass = renderPass;

		// reset builder info
		pipelineBuilder = {};

		// build the pipeline layout that controls the inputs/outputs of the shader
		// we are not using descriptor sets or other systems yet, so no need to use anything other than empty default
		VkPipelineLayoutCreateInfo pipeline_layout_info = vkinit::pipeline_layout_create_info();

		// TODO: add default push_constants, for instance: MVP
		int constants_num = layout.description.constants.size();
		if (constants_num) {
			pipeline_layout_info.pPushConstantRanges = layout.description.constants.data();
			pipeline_layout_info.pushConstantRangeCount = constants_num;
		}

		VK_CHECK(vkCreatePipelineLayout(device->_device, &pipeline_layout_info, nullptr, &pipelineLayout));

		// vertex input controls how to read vertices from vertex buffers. We arent using it yet
		pipelineBuilder._vertexInputInfo = vkinit::vertex_input_state_create_info(layout);

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

		// use the triangle layout we created
		pipelineBuilder._pipelineLayout = pipelineLayout;
	}

	void Pipeline::add_pipeline(const char* vertexShaderPath, const char* fragmentShaderPath)
	{
		try {
			VkShaderModule fragShader = load_shader_module(fragmentShaderPath);
			VkShaderModule vertexShader = load_shader_module(vertexShaderPath);

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
		catch (ShaderError error) {
			std::cout << error.desc << "\n";
		}
	}

	void Pipeline::Destroy()
	{
		for (auto iter = pipelines.begin(); iter != pipelines.end(); ++iter) {
			vkDestroyPipeline(device->_device, *iter, nullptr);
		}

		vkDestroyPipelineLayout(device->_device, pipelineLayout, nullptr);
	}

	void Pipeline::bind(VkCommandBuffer& cmd, uint32_t& selectedPipelineIndex)
	{
		vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines[selectedPipelineIndex]);
	}

	void Pipeline::upload_pushConstants(VkCommandBuffer& cmd)
	{
		// TODO: add default push_constants, for instance: MVP
		auto constants = &layout->description.constants;
		auto constants_data = &layout->description.constants_data;
		uint32_t offset = 0;
		for (uint32_t i = 0, length = constants->size(); i < length; ++i) {
			//upload the matrix to the GPU via push constants
			uint32_t size = (*constants)[i].size;
			vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, offset, size, (*constants_data)[i]);
			offset += size;
		}
	}
}
