#pragma once
#include "VK_HEADER.h"

namespace LearnVulkan {
	class Renderer {
	public:
		VkInstance _instance; // Vulkan library handle
		VkDebugUtilsMessengerEXT _debug_messenger; // Vulkan debug output handle
		VkPhysicalDevice _chosenGPU; // GPU chosen as the default device
		VkDevice _device; // Vulkan device for commands
		VkSurfaceKHR _surface; // Vulkan window surface
	};
}
