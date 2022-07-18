#include "Device.h"

namespace LearnVulkan {
	constexpr bool bUseValidationLayers = true;

	Device::Device() {}

	Device::~Device() {}

	void Device::Destroy()
	{
		// make sure the gpu has stopped doing its things
		vkDeviceWaitIdle(_device);

		vkDestroySurfaceKHR(_instance, _surface, nullptr);

		vkDestroyDevice(_device, nullptr);
		vkb::destroy_debug_utils_messenger(_instance, _debug_messenger);
		vkDestroyInstance(_instance, nullptr);
	}

	void Device::init_vulkan(Window& w)
	{
		window = &w;

		auto size = w.getWindowSize();
		_windowExtent = { size.width, size.height };

		vkb::InstanceBuilder builder;

		// make the vulkan instance, with basic debug features
		auto inst_ret = builder.set_app_name("Example Vulkan Application")
			.request_validation_layers(bUseValidationLayers)
			.use_default_debug_messenger()
			.require_api_version(1, 1, 0)
			.build();

		vkb::Instance vkb_inst = inst_ret.value();

		// grab the instance 
		_instance = vkb_inst.instance;
		_debug_messenger = vkb_inst.debug_messenger;

		w.getSurface(_instance, _surface);

		// use vkbootstrap to select a gpu. 
		// We want a gpu that can write to the SDL surface and supports vulkan 1.2
		vkb::PhysicalDeviceSelector selector{ vkb_inst };
		vkb::PhysicalDevice physicalDevice = selector
			.set_minimum_version(1, 1)
			.set_surface(_surface)
			.select()
			.value();

		// create the final vulkan device

		vkb::DeviceBuilder deviceBuilder{ physicalDevice };

		vkb::Device vkbDevice = deviceBuilder.build().value();

		// Get the VkDevice handle used in the rest of a vulkan application
		_device = vkbDevice.device;
		_chosenGPU = physicalDevice.physical_device;

		// use vkbootstrap to get a Graphics queue
		_graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();

		_graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();
	}

	void Device::submit(VkSubmitInfo& submitInfo, VkFence& renderFence)
	{
		// submit command buffer to the queue and execute it.
		// _renderFence will now block until the graphic commands finish execution
		VK_CHECK(vkQueueSubmit(_graphicsQueue, 1, &submitInfo, renderFence));
	}

	void Device::present(VkPresentInfoKHR& presentInfo)
	{
		VK_CHECK(vkQueuePresentKHR(_graphicsQueue, &presentInfo));
	}
}
