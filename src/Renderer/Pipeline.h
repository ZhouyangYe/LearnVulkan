#pragma once
#include "VK_HEADER.h"
#include "Device.h"

namespace LearnVulkan {
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
			const char* desc;

			ShaderError(const char* desc) : desc(desc) {}
		};

		std::vector<VkPipeline> pipelines;

		Pipeline();
		~Pipeline();

		void bind(VkCommandBuffer& cmd);
		void render(VkCommandBuffer& cmd);

		VkShaderModule load_shader_module(const char* filePath);
		void init_pipeline(Device* device, VkRenderPass* renderPass);
		void Destroy();
		void add_pipeline(const char* vertexShaderPath, const char* fragmentShaderPath);
		VkPipeline& getSelectedPipeline();
		const int& getSelectedPipelineIndex();
		void setSelectedPipeline(int index);
	private:
		Device* device;
		VkRenderPass* _renderPass;
		VkPipelineLayout pipelineLayout;
		//build the stage-create-info for both vertex and fragment stages. This lets the pipeline know the shader modules per stage
		PipelineBuilder pipelineBuilder;
		uint32_t selectedPipelineIndex = 0;
	};
}
