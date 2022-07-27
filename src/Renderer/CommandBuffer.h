#pragma once
#include "VK_HEADER.h"
#include "Device.h"
#include "Helper.h"

namespace LearnVulkan {
	class CommandBuffer {
	public:
		VkRenderPass _renderPass;
		VkCommandBuffer _mainCommandBuffer;

		CommandBuffer(Device* device);
		~CommandBuffer();

		void Destroy();

		void init_commands();
		void init_renderpass(float clearColor[4], VkFormat swapchainImageFormat, VkFormat depthFormat);
		
		void begin_command();
		void end_command();
		void begin_renderPass(VkSwapchainKHR swapChain, uint32_t swapchainImageIndex, std::vector<VkFramebuffer>& _framebuffers);
		void end_renderPass();

		void bind(VkPipeline& pipeline);
	private:
		Device* device;
		VkClearColorValue clearColor;

		VkCommandPool _commandPool;
	};
}
