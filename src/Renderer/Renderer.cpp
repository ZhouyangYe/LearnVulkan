#include "Renderer.h"

namespace LearnVulkan {
	glm::mat4 Renderer::projection_view;

	Renderer::Renderer()
		: commandBuffer(&device), swapChain(&device), sync(&device)
	{}

	Renderer::~Renderer() {}

	void Renderer::Init(RendererProps& props)
	{
		device.init_vulkan(props.window);

		sync.init_sync_structures();

		swapChain.init_swapchain();

		commandBuffer.init_commands();

		commandBuffer.init_renderpass(props.clearColor, swapChain._swapchainImageFormat, swapChain._depthFormat);

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

	void Renderer::draw_renderables(VkCommandBuffer cmd, Renderable* first, int count)
	{
		VkPipeline lastPipeline = nullptr;
		VkBuffer lastBuffer = nullptr;
		for (int i = 0; i < count; i++)
		{
			Renderable& object = first[i];

			//only bind the pipeline if it doesn't match with the already bound one
			if (object.pipeline != lastPipeline) {
				commandBuffer.bind(object.pipeline);
				lastPipeline = object.pipeline;
			}

			//only bind the mesh if it's a different one from last bind
			if (object.buffer._buffer != lastBuffer) {
				//bind the mesh vertex buffer with offset 0
				VkDeviceSize offset = 0;
				vkCmdBindVertexBuffers(cmd, 0, 1, &object.buffer._buffer, &offset);
				lastBuffer = object.buffer._buffer;
			}

			// upload push constants
			MeshPushConstants constants;
			// calculate mvp matrix
			constants.mvp = Renderer::projection_view * object.model;
			upload_pushConstants(cmd, object.pipelineLayout, &constants);

			//we can now draw
			vkCmdDraw(cmd, object.vertice_num, 1, 0, 0);
		}
	}

	void Renderer::Draw(std::vector<Renderable>& objects)
	{
		sync.sync_gpu();

		commandBuffer.begin_command();

		swapChain.request_imgIndex(sync._presentSemaphore);
		commandBuffer.begin_renderPass(swapChain._swapchain, swapChain.swapchainImageIndex, swapChain._framebuffers);

		draw_renderables(commandBuffer._mainCommandBuffer, objects.data(), objects.size());

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

	void Renderer::upload_pushConstants(VkCommandBuffer cmd, VkPipelineLayout pipelineLayout, const void* data)
	{
		uint32_t offset = 0;
		// upload the matrix to the GPU via push constants
		uint32_t size = sizeof(MeshPushConstants);
		vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, offset, size, data);
		// offset += size;
	}

	void Renderer::setViewTransform(glm::mat4& view, glm::mat4& projection)
	{
		projection_view = projection * view;
	}
}
