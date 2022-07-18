#include "Synchronization.h"

namespace LearnVulkan {
	Synchronization::Synchronization(Device* device) : device(device) {}

	Synchronization::~Synchronization() {}

	void Synchronization::Destroy()
	{
		// destroy sync objects
		vkDestroyFence(device->_device, _renderFence, nullptr);
		vkDestroySemaphore(device->_device, _renderSemaphore, nullptr);
		vkDestroySemaphore(device->_device, _presentSemaphore, nullptr);
	}

	void Synchronization::init_sync_structures()
	{
		// create syncronization structures
		// one fence to control when the gpu has finished rendering the frame,
		// and 2 semaphores to syncronize rendering with swapchain
		// we want the fence to start signalled so we can wait on it on the first frame
		VkFenceCreateInfo fenceCreateInfo = vkinit::fence_create_info(VK_FENCE_CREATE_SIGNALED_BIT);

		VK_CHECK(vkCreateFence(device->_device, &fenceCreateInfo, nullptr, &_renderFence));

		VkSemaphoreCreateInfo semaphoreCreateInfo = vkinit::semaphore_create_info();

		VK_CHECK(vkCreateSemaphore(device->_device, &semaphoreCreateInfo, nullptr, &_presentSemaphore));
		VK_CHECK(vkCreateSemaphore(device->_device, &semaphoreCreateInfo, nullptr, &_renderSemaphore));
	}

	void Synchronization::sync_gpu()
	{
		// wait until the gpu has finished rendering the last frame. Timeout of 1 second
		VK_CHECK(vkWaitForFences(device->_device, 1, &_renderFence, true, 1000000000));
		VK_CHECK(vkResetFences(device->_device, 1, &_renderFence));
	}
}
