#include "Renderer.h"

namespace LearnVulkan {

	Renderer::Renderer()
		: commandBuffer(&device), swapChain(&device), sync(&device)
	{}

	Renderer::~Renderer() {}

	void Renderer::Init(Window& window)
	{
		device.init_vulkan(window);

		sync.init_sync_structures();

		swapChain.init_swapchain();

		commandBuffer.init_commands();

		commandBuffer.init_renderpass(swapChain._swapchainImageFormat);

		swapChain.init_framebuffers(commandBuffer._renderPass);

		// everything went fine
		_isInitialized = true;
	}

	void Renderer::Destroy()
	{
		if (_isInitialized) {
			// destroy command buffers
			commandBuffer.Destroy();

			// destroy synchronization structures
			sync.Destroy();

			// destroy swap chain
			swapChain.Destroy();

			// destroy device
			device.Destroy();
		}
	}

	void Renderer::Draw()
	{
		sync.sync_gpu();

		commandBuffer.begin_command();

		commandBuffer.begin_renderPass(swapChain._swapchain, sync._presentSemaphore, swapChain.swapchainImageIndex, device._windowExtent, swapChain._framebuffers);

		// once we start adding rendering commands, they will go here

		commandBuffer.end_renderPass();

		commandBuffer.end_command();

		// prepare the submission to the queue.
		// we want to wait on the _presentSemaphore, as that semaphore is signaled when the swapchain is ready
		// we will signal the _renderSemaphore, to signal that rendering has finished
		VkSubmitInfo submit = vkinit::submit_info(&commandBuffer._mainCommandBuffer);
		VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		submit.pWaitDstStageMask = &waitStage;

		submit.waitSemaphoreCount = 1;
		submit.pWaitSemaphores = &sync._presentSemaphore;

		submit.signalSemaphoreCount = 1;
		submit.pSignalSemaphores = &sync._renderSemaphore;

		device.submit(submit, sync._renderFence);

		// prepare present
		// this will put the image we just rendered to into the visible window.
		// we want to wait on the _renderSemaphore for that,
		// as its necessary that drawing commands have finished before the image is displayed to the user
		VkPresentInfoKHR presentInfo = vkinit::present_info();

		presentInfo.pSwapchains = &swapChain._swapchain;
		presentInfo.swapchainCount = 1;

		presentInfo.pWaitSemaphores = &sync._renderSemaphore;
		presentInfo.waitSemaphoreCount = 1;

		presentInfo.pImageIndices = &swapChain.swapchainImageIndex;

		device.present(presentInfo);
	}
}
