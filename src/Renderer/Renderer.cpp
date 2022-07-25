#include "Renderer.h"

namespace LearnVulkan {
	glm::mat4 Renderer::projection_view;

	Renderer::Renderer()
		: commandBuffer(&device), swapChain(&device), sync(&device)
	{}

	Renderer::~Renderer() {}

	void Renderer::initPipeline(Pipeline& pipeline, VertexLayout& layout)
	{
		pipeline.init_pipeline(&device, &commandBuffer._renderPass, layout);
	}

	void Renderer::Init(RendererProps& props)
	{
		device.init_vulkan(props.window);

		sync.init_sync_structures();

		swapChain.init_swapchain();

		commandBuffer.init_commands();

		commandBuffer.init_renderpass(props.clearColor, swapChain._swapchainImageFormat);

		swapChain.init_framebuffers(commandBuffer._renderPass);

		// everything went fine
		_isInitialized = true;
	}

	void Renderer::Wait()
	{
		if (_isInitialized) {
			// make sure the gpu has stopped doing its things
			vkDeviceWaitIdle(device._device);
		}
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

	void Renderer::Draw(Pipeline& pipeline, VertexBuffer& vBuffer, uint32_t& selectedPipelineIndex, glm::mat4& model, uint32_t& vertice_num)
	{
		sync.sync_gpu();

		commandBuffer.begin_command();

		swapChain.request_imgIndex(sync._presentSemaphore);
		commandBuffer.begin_renderPass(swapChain._swapchain, swapChain.swapchainImageIndex, swapChain._framebuffers);

		// rendering commands
		// bind pipeline
		pipeline.bind(commandBuffer._mainCommandBuffer, selectedPipelineIndex);

		// bind the mesh vertex buffer with offset 0
		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(commandBuffer._mainCommandBuffer, 0, 1, &vBuffer._buffer, &offset);

		// upload push constants
		// calculate mvp matrix
		MeshPushConstants constants;
		constants.mvp = Renderer::projection_view * model;
		pipeline.upload_pushConstants(commandBuffer._mainCommandBuffer, &constants);

		// draw
		vkCmdDraw(commandBuffer._mainCommandBuffer, vertice_num, 1, 0, 0);

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
		// this will put the image we just rendered into the visible window.
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

	void Renderer::setViewTransform(glm::mat4& view, glm::mat4& projection)
	{
		projection_view = projection * view;
	}
}
