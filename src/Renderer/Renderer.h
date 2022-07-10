#pragma once
#include <VkBootstrap.h>
#include "VK_HEADER.h"
#include "Helper.h"
#include "Core/Common.h"
#include "Window/Window.h"

namespace LearnVulkan {
	class Renderer {
	public:
		bool _isInitialized{ false };
		int _frameNumber{ 0 };

		VkExtent2D _windowExtent{ 1700 , 900 };

		VkInstance _instance;
		VkDebugUtilsMessengerEXT _debug_messenger;
		VkPhysicalDevice _chosenGPU;
		VkDevice _device;

		VkSemaphore _presentSemaphore, _renderSemaphore;
		VkFence _renderFence;

		VkQueue _graphicsQueue;
		uint32_t _graphicsQueueFamily;

		VkCommandPool _commandPool;
		VkCommandBuffer _mainCommandBuffer;

		VkRenderPass _renderPass;

		VkSurfaceKHR _surface;
		VkSwapchainKHR _swapchain;
		VkFormat _swachainImageFormat;

		std::vector<VkFramebuffer> _framebuffers;
		std::vector<VkImage> _swapchainImages;
		std::vector<VkImageView> _swapchainImageViews;

		//initializes everything in the engine
		void Init(Window& window);
		//shuts down the engine
		void Destroy();
		//draw loop
		void Draw();
	private:
		void init_vulkan(Window& window);
		void init_swapchain();
		void init_default_renderpass();
		void init_framebuffers();
		void init_commands();
		void init_sync_structures();
	};
}
