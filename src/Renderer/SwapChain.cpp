#include "SwapChain.h"

namespace LearnVulkan {
	SwapChain::SwapChain(Device* device)
		: device(device) {}

	SwapChain::~SwapChain() {}

	void SwapChain::Destroy()
	{
		vkDestroySwapchainKHR(device->_device, _swapchain, nullptr);

		//destroy swapchain resources
		for (int i = 0; i < _framebuffers.size(); i++) {
			vkDestroyFramebuffer(device->_device, _framebuffers[i], nullptr);

			vkDestroyImageView(device->_device, _swapchainImageViews[i], nullptr);
		}
	}

	void SwapChain::init_swapchain()
	{
		vkb::SwapchainBuilder swapchainBuilder{ device->_chosenGPU, device->_device, device->_surface };

		vkb::Swapchain vkbSwapchain = swapchainBuilder
			.use_default_format_selection()
			//use vsync present mode
			.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
			.set_desired_extent((device->_windowExtent).width, (device->_windowExtent).height)
			.build()
			.value();

		//store swapchain and its related images
		_swapchain = vkbSwapchain.swapchain;
		_swapchainImages = vkbSwapchain.get_images().value();
		_swapchainImageViews = vkbSwapchain.get_image_views().value();

		_swapchainImageFormat = vkbSwapchain.image_format;
	}

	void SwapChain::init_framebuffers(VkRenderPass& renderPass)
	{
		//create the framebuffers for the swapchain images. This will connect the render-pass to the images for rendering
		VkFramebufferCreateInfo fb_info = vkinit::framebuffer_create_info(renderPass, device->_windowExtent);

		const uint32_t swapchain_imagecount = _swapchainImages.size();
		_framebuffers = std::vector<VkFramebuffer>(swapchain_imagecount);

		for (int i = 0; i < swapchain_imagecount; i++) {

			fb_info.pAttachments = &_swapchainImageViews[i];
			VK_CHECK(vkCreateFramebuffer(device->_device, &fb_info, nullptr, &_framebuffers[i]));
		}
	}

	void SwapChain::request_imgIndex(VkSemaphore& semaphore)
	{
		//request image from the swapchain
		VK_CHECK(vkAcquireNextImageKHR(device->_device, _swapchain, 1000000000, semaphore, nullptr, &swapchainImageIndex));
	}
}
