#include "Synchronization.h"

namespace LearnVulkan {
	Synchronization::Synchronization(Device* device) : device(device) {}

	Synchronization::~Synchronization() {}

	void Synchronization::Destroy()
	{
		// destroy sync objects
		vkDestroyFence(device->_device, _uploadFence, nullptr);

		for (auto iter = locks.begin(); iter != locks.end(); ++iter) {
			vkDestroyFence(device->_device, iter->_renderFence, nullptr);
			vkDestroySemaphore(device->_device, iter->_renderSemaphore, nullptr);
			vkDestroySemaphore(device->_device, iter->_presentSemaphore, nullptr);
		}
	}

	void Synchronization::init_sync_structures(uint32_t num)
	{
		locks.resize(num);

		VkFenceCreateInfo uploadFenceCreateInfo = vkinit::fence_create_info();
		VK_CHECK(vkCreateFence(device->_device, &uploadFenceCreateInfo, nullptr, &_uploadFence));

		// create syncronization structures
		// one fence to control when the gpu has finished rendering the frame,
		// and 2 semaphores to syncronize rendering with swapchain
		// we want the fence to start signalled so we can wait on it on the first frame
		VkFenceCreateInfo fenceCreateInfo = vkinit::fence_create_info(VK_FENCE_CREATE_SIGNALED_BIT);

		VkSemaphoreCreateInfo semaphoreCreateInfo = vkinit::semaphore_create_info();

		for (int i = 0; i < num; i++) {
			VK_CHECK(vkCreateFence(device->_device, &fenceCreateInfo, nullptr, &locks[i]._renderFence));

			VK_CHECK(vkCreateSemaphore(device->_device, &semaphoreCreateInfo, nullptr, &locks[i]._presentSemaphore));
			VK_CHECK(vkCreateSemaphore(device->_device, &semaphoreCreateInfo, nullptr, &locks[i]._renderSemaphore));
		}
	}

	void Synchronization::sync_gpu()
	{
		// wait until the gpu has finished rendering the last frame. Timeout of 1 second
		VK_CHECK(vkWaitForFences(device->_device, 1, &_renderFence, true, 1000000000));
		VK_CHECK(vkResetFences(device->_device, 1, &_renderFence));
	}

	void Synchronization::setLockSet(uint32_t index)
	{
		_renderFence = locks[index]._renderFence;
		_renderSemaphore = locks[index]._renderSemaphore;
		_presentSemaphore = locks[index]._presentSemaphore;
	}
}
