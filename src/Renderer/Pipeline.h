#pragma once
#include "VK_HEADER.h"
#include "Device.h"
#include "VertexLayout.h"
#include "Helper.h"
#include "Events/Error.h"

namespace LearnVulkan {
	struct MeshPushConstants {
		glm::vec4 data;
		glm::mat4 mvp;
	};

	class PipelineBuilder {
	public:
		std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
		VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
		VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
		VkViewport _viewport;
		VkRect2D _scissor;
		VkPipelineRasterizationStateCreateInfo _rasterizer;
		VkPipelineColorBlendAttachmentState _colorBlendAttachment;
		VkPipelineMultisampleStateCreateInfo _multisampling;
		VkPipelineLayout _pipelineLayout;

		VkPipeline build_pipeline(VkDevice device, VkRenderPass pass);
	};

	class Pipeline {
	public:
		std::vector<VkPipelineLayout> layouts;
		std::vector<VkPipeline> pipelines;

		Pipeline(Device* device, VkRenderPass* renderPass);
		~Pipeline();

		uint32_t constant_offset = 0;
		Pipeline& add_constant(uint32_t size);

		VkShaderModule load_shader_module(const char* filePath);
		Pipeline& init_layout();
		Pipeline& init_vertex_layout(VertexLayout& layout);
		Pipeline& add_pipeline(std::string shaderName);

		void Destroy();
	private:
		Device* device;
		VkRenderPass* _renderPass;
		// build the stage-create-info for both vertex and fragment stages. This lets the pipeline know the shader modules per stage
		PipelineBuilder pipelineBuilder;
		std::vector<VkPushConstantRange> constants;
	};
}
