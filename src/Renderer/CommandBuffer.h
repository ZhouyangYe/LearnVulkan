#pragma once
#include "VK_HEADER.h"
#include "Device.h"

namespace LearnVulkan {
	class CommandBuffer {
	public:
		VkRenderPass _renderPass;
		VkCommandBuffer _mainCommandBuffer;

		CommandBuffer(Device* device);
		~CommandBuffer();

		void Destroy();

		void init_commands();
		void init_renderpass(VkFormat& swapchainImageFormat);
		
		void begin_command();
		void end_command();
		void begin_renderPass(VkSwapchainKHR& swapChain, VkSemaphore& presentSemaphore, uint32_t& swapchainImageIndex, VkExtent2D& _windowExtent, std::vector<VkFramebuffer>& _framebuffers);
		void end_renderPass();
	private:
		Device* device;

		float _frameNumber{ 0.0f };

		VkCommandPool _commandPool;
	};
}
