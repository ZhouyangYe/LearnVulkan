#pragma once
#include "VK_HEADER.h"
#include "Buffer.h"

namespace LearnVulkan {
	struct GPUData {
		const void* data;
		uint64_t size;
		uint64_t offset = 0;
	};

	class Device {
	public:
		VmaAllocator _allocator; // vma lib allocator
		VkDevice _device;
		uint32_t _graphicsQueueFamily;
		VkQueue _graphicsQueue;
		VkPhysicalDevice _chosenGPU;
		VkPhysicalDeviceProperties _gpuProperties;
		Window* window = NULL;

		VkExtent2D _windowExtent{ 1700 , 900 };

		VkSurfaceKHR _surface;

		Device();
		~Device();

		void Destroy();

		uint64_t pad_uniform_buffer_size(uint64_t originalSize);

		void init_vulkan(Window* window);
		Buffer create_buffer(uint64_t size, VkBufferUsageFlags bufferUsage, VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_AUTO, VmaAllocationCreateFlags flags = 0);
		void upload_data(Buffer& buffer, GPUData& data);
		void destroy_buffer(Buffer& buffer);

		void submit(VkSubmitInfo& submitInfo, VkFence renderFence);
		void present(VkPresentInfoKHR& presentInfo);
	private:
		VkInstance _instance;
		VkDebugUtilsMessengerEXT _debug_messenger;
	};
}
