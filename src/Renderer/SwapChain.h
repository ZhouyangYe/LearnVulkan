#pragma once
#include "VK_HEADER.h"
#include "Device.h"
#include "Helper.h"
#include "Buffer.h"

namespace LearnVulkan {
	class SwapChain {
	public:
		uint32_t swapchainImageIndex;
		VkFormat _swapchainImageFormat;

		VkImageView _depthImageView;
		ImageBuffer _depthImage;
		//the format for the depth image
		VkFormat _depthFormat;

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
