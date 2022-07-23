#pragma once
#include "VK_HEADER.h"
#include "Device.h"
#include "Helper.h"

namespace LearnVulkan {
	class SwapChain {
	public:
		uint32_t swapchainImageIndex;
		VkFormat _swapchainImageFormat;

		VkSwapchainKHR _swapchain;

		std::vector<VkFramebuffer> _framebuffers;

		SwapChain(Device* device);
		~SwapChain();

		void Destroy();

		void init_swapchain();
		void init_framebuffers(VkRenderPass& renderPass);
		void request_imgIndex(VkSemaphore& semaphore);
	private:
		Device* device;

		std::vector<VkImage> _swapchainImages;
		std::vector<VkImageView> _swapchainImageViews;
	};
}
