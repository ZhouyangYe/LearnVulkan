#pragma once
#include "VK_HEADER.h"
#include "Device.h"
#include "VertexLayout.h"
#include "Helper.h"
#include "Descriptor.h"
#include "Events/Error.h"

namespace LearnVulkan {
	struct MeshPushConstants {
		glm::vec4 data;
		glm::mat4 renderMatrix;
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
		VkPipelineDepthStencilStateCreateInfo _depthStencil;

		VkPipeline build_pipeline(VkDevice device, VkRenderPass pass);
	};

	class Pipeline {
	public:
		std::vector<VkPipelineLayout> layouts;
		std::vector<VkPipeline> pipelines;

		Pipeline(Device* device, VkRenderPass* renderPass, Descriptor* descriptor);
		~Pipeline();

		uint32_t constant_offset = 0;
		Pipeline& add_constant(uint32_t size);

		VkShaderModule load_shader_module(const char* filePath);
		Pipeline& init_layout(std::vector<VkDescriptorSetLayout>& descriptorLayouts);
		Pipeline& init_vertex_layout(VertexLayout& layout);
		Pipeline& add_pipeline(std::string shaderName);

		void Destroy();
	private:
		Device* device;
		VkRenderPass* _renderPass;
		Descriptor* descriptor;
		// build the stage-create-info for both vertex and fragment stages. This lets the pipeline know the shader modules per stage
		PipelineBuilder pipelineBuilder;
		std::vector<VkPushConstantRange> constants;
	};
}
