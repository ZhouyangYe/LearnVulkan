#pragma once
#include "VK_HEADER.h"
#include "Device.h"

namespace LearnVulkan {
	class Synchronization {
	public:
		VkSemaphore _presentSemaphore, _renderSemaphore;
		VkFence _renderFence;

		Synchronization(Device* device);
		~Synchronization();

		void Destroy();

		void init_sync_structures();

		void sync_gpu();
	private:
		Device* device;
	};
}
