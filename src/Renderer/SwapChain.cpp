#include "SwapChain.h"

namespace LearnVulkan {
	SwapChain::SwapChain(Device* device)
		: device(device) {}

	SwapChain::~SwapChain() {}

	void SwapChain::Destroy()
	{
		vkDestroyImageView(device->_device, _depthImageView, nullptr);
		vmaDestroyImage(device->_allocator, _depthImage._image, _depthImage._allocation);

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

		// create depth image
		// depth image size will match the window
		VkExtent3D depthImageExtent = {
			device->_windowExtent.width,
			device->_windowExtent.height,
			1
		};

		// hardcoding the depth format to 32 bit float
		_depthFormat = VK_FORMAT_D32_SFLOAT;

		// the depth image will be an image with the format we selected and Depth Attachment usage flag
		VkImageCreateInfo dimg_info = vkinit::image_create_info(_depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, depthImageExtent);

		// for the depth image, we want to allocate it from GPU local memory
		VmaAllocationCreateInfo dimg_allocinfo = {};
		dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		dimg_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		// allocate and create the image
		vmaCreateImage(device->_allocator, &dimg_info, &dimg_allocinfo, &_depthImage._image, &_depthImage._allocation, nullptr);

		//build an image-view for the depth image to use for rendering
		VkImageViewCreateInfo dview_info = vkinit::imageview_create_info(_depthFormat, _depthImage._image, VK_IMAGE_ASPECT_DEPTH_BIT);

		VK_CHECK(vkCreateImageView(device->_device, &dview_info, nullptr, &_depthImageView));
	}

	void SwapChain::init_framebuffers(VkRenderPass& renderPass)
	{
		//create the framebuffers for the swapchain images. This will connect the render-pass to the images for rendering
		VkFramebufferCreateInfo fb_info = vkinit::framebuffer_create_info(renderPass, device->_windowExtent);

		const uint32_t swapchain_imagecount = _swapchainImages.size();
		_framebuffers = std::vector<VkFramebuffer>(swapchain_imagecount);

		for (int i = 0; i < swapchain_imagecount; i++) {
			VkImageView attachments[2];
			attachments[0] = _swapchainImageViews[i];
			attachments[1] = _depthImageView;

			fb_info.pAttachments = attachments;
			fb_info.attachmentCount = 2;

			VK_CHECK(vkCreateFramebuffer(device->_device, &fb_info, nullptr, &_framebuffers[i]));
		}
	}

	void SwapChain::request_imgIndex(VkSemaphore& semaphore)
	{
		//request image from the swapchain
		VK_CHECK(vkAcquireNextImageKHR(device->_device, _swapchain, 1000000000, semaphore, nullptr, &swapchainImageIndex));
	}
}
