#pragma once
#include "VK_HEADER.h"

namespace LearnVulkan {
	class Device {
	public:
		VkDevice _device;
		uint32_t _graphicsQueueFamily;
		VkPhysicalDevice _chosenGPU;
		Window* window = NULL;

		VkExtent2D _windowExtent{ 1700 , 900 };

		VkSurfaceKHR _surface;

		Device();
		~Device();

		void Destroy();

		void init_vulkan(Window* window);

		void submit(VkSubmitInfo& submitInfo, VkFence& renderFence);
		void present(VkPresentInfoKHR& presentInfo);
	private:
		VkInstance _instance;
		VkDebugUtilsMessengerEXT _debug_messenger;

		VkQueue _graphicsQueue;
	};
}
