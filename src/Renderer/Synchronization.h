#pragma once
#include "VK_HEADER.h"
#include "Device.h"
#include "Helper.h"

namespace LearnVulkan {
	class Synchronization {
	public:
		struct LockSet {
			VkSemaphore _presentSemaphore, _renderSemaphore;
			VkFence _renderFence;
		};
		std::vector<LockSet> locks;

		VkSemaphore _presentSemaphore, _renderSemaphore;
		VkFence _renderFence;

		Synchronization(Device* device);
		~Synchronization();

		void Destroy();

		void init_sync_structures(uint32_t num);

		void sync_gpu();

		void setLockSet(uint32_t index);
	private:
		Device* device;
	};
}
