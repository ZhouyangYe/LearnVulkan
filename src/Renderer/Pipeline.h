#pragma once
#include "VK_HEADER.h"
#include "Device.h"
#include "VertexLayout.h"
#include "Helper.h"

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
		struct ShaderError {
			std::string desc;

			ShaderError(std::string desc) : desc(desc) {}
		};

		VertexLayout* layout;
		std::vector<VkPipeline> pipelines;
		std::vector<VertexLayout*> layouts;

		Pipeline();
		~Pipeline();

		void bind(VkCommandBuffer& cmd, uint32_t& selectedPipelineIndex);
		void upload_pushConstants(VkCommandBuffer& cmd, const void* data);

		uint32_t constant_offset = 0;
		Pipeline& add_constant(uint32_t size);

		VkShaderModule load_shader_module(const char* filePath);
		void init_pipeline(Device* device, VkRenderPass* renderPass, VertexLayout& layout);
		void Destroy();
		void add_pipeline(const char* vertexShaderPath, const char* fragmentShaderPath);
	private:
		Device* device;
		VkRenderPass* _renderPass;
		VkPipelineLayout pipelineLayout;
		//build the stage-create-info for both vertex and fragment stages. This lets the pipeline know the shader modules per stage
		PipelineBuilder pipelineBuilder;
		std::vector<VkPushConstantRange> constants;
	};
}
