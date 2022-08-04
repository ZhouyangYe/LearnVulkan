#pragma once
#include "VK_HEADER.h"
#include "Buffer.h"

namespace LearnVulkan {
	class Device {
	public:
		VmaAllocator _allocator; //vma lib allocator
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
		Buffer create_buffer(uint64_t size, VkBufferUsageFlags bufferUsage, VmaMemoryUsage memoryUsage);
		void upload_data(Buffer& buffer, const void* data, uint64_t size);
		void upload_vertex_data(Buffer& buffer, const void* data, uint64_t size);
		void destroy_buffer(Buffer& buffer);

		void submit(VkSubmitInfo& submitInfo, VkFence renderFence);
		void present(VkPresentInfoKHR& presentInfo);
	private:
		VkInstance _instance;
		VkDebugUtilsMessengerEXT _debug_messenger;

		VkQueue _graphicsQueue;
	};
}
