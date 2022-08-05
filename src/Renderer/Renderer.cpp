#include "Renderer.h"

#define FRAME_OVERLAP 2

namespace LearnVulkan {
	Renderer::CameraData Renderer::camData;
	Renderer::SceneData Renderer::sceneData;

	Renderer::Renderer()
		: commandBuffer(&device), swapChain(&device), sync(&device), descriptor(&device)
	{}

	Renderer::~Renderer() {}

	void Renderer::Init(RendererProps& props)
	{
		device.init_vulkan(props.window);

		sync.init_sync_structures(FRAME_OVERLAP);

		swapChain.init_swapchain();

		commandBuffer.init_commands(FRAME_OVERLAP);

		commandBuffer.init_renderpass(props.clearColor, swapChain._swapchainImageFormat, swapChain._depthFormat);

		swapChain.init_framebuffers(commandBuffer._renderPass);

		descriptor.init_uniforms(FRAME_OVERLAP, sizeof(CameraData), sizeof(SceneData));

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

			// destroy uniform
			descriptor.Destroy();

			// destroy device
			device.Destroy();
		}
	}

	void Renderer::draw_renderables(VkCommandBuffer cmd, Renderable* first, int count)
	{
		VkPipeline lastPipeline = nullptr;
		VkBuffer lastBuffer = nullptr;
		VkPipelineLayout lastLayout = nullptr;

		uint32_t offsets[2] = { descriptor.alignedCameraDataSize * frameIndex, descriptor.alignedSceneDataSize * frameIndex };
		GPUData camera_data{ &camData, sizeof(CameraData), offsets[0] };
		//offset for our scene buffer
		GPUData scene_data{ &sceneData, sizeof(SceneData), offsets[1] };
		descriptor.update_uniform_data(camera_data, scene_data);

		for (int i = 0; i < count; i++)
		{
			Renderable& object = first[i];

			// only bind the pipeline if it doesn't match with the already bound one
			if (object.pipeline != lastPipeline) {
				commandBuffer.bind(object.pipeline);
				lastPipeline = object.pipeline;
			}

			if (object.pipelineLayout != lastLayout) {
				//bind the descriptor set when changing pipeline
				vkCmdBindDescriptorSets(commandBuffer._mainCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object.pipelineLayout, 0, descriptor._descriptorSets.size(), descriptor._descriptorSets.data(), 2, offsets);
				lastLayout = object.pipelineLayout;
			}

			// upload push constants
			MeshPushConstants constants;
			// calculate mvp matrix
			constants.renderMatrix = object.model;
			upload_pushConstants(cmd, object.pipelineLayout, &constants);

			// only bind the mesh if it's a different one from last bind
			if (object.vertex_buffer._buffer != lastBuffer) {
				// bind the mesh vertex buffer with offset 0
				VkDeviceSize offset = 0;
				vkCmdBindVertexBuffers(cmd, 0, 1, &object.vertex_buffer._buffer, &offset);
				lastBuffer = object.vertex_buffer._buffer;
			}

			// we can now draw
			vkCmdDraw(cmd, object.vertice_num, 1, 0, 0);
		}
	}

	void Renderer::Draw()
	{
		commandBuffer.setCommand(frameIndex);
		sync.setLockSet(frameIndex);

		sync.sync_gpu();

		commandBuffer.begin_command();

		swapChain.request_imgIndex(sync._presentSemaphore);
		commandBuffer.begin_renderPass(swapChain._swapchain, swapChain.swapchainImageIndex, swapChain._framebuffers);

		draw_renderables(commandBuffer._mainCommandBuffer, renderable_objects.data(), renderable_objects.size());

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

		// reset
		renderable_objects.clear();
		frameIndex = (frameIndex + 1) % FRAME_OVERLAP;
	}

	void Renderer::upload_pushConstants(VkCommandBuffer cmd, VkPipelineLayout pipelineLayout, const void* data)
	{
		uint32_t offset = 0;
		// upload the matrix to the GPU via push constants
		uint32_t size = sizeof(MeshPushConstants);
		vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, offset, size, data);
		// offset += size;
	}

	void Renderer::setViewTransform(glm::mat4& v, glm::mat4& p)
	{
		camData.projection = p;
		camData.view = v;
		camData.projection_view = p * v;
	}

	void Renderer::submit(Buffer& buffer, VkPipelineLayout pipelineLayout, VkPipeline pipeline, uint32_t vertice_num, glm::mat4& model)
	{
		// TODO: design a data structure to optimize this
		renderable_objects.emplace_back(buffer, pipelineLayout, pipeline, vertice_num, model);
	}

	void Renderer::upload_vertex_data(Buffer& buffer, GPUData& d)
	{
		// TODO: VMA_MEMORY_USAGE_CPU_ONLY is deprecated, use VMA_MEMORY_USAGE_AUTO and flags
		Buffer cpu_temp_buffer = device.create_buffer(d.size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
		device.upload_data(cpu_temp_buffer, d);
		buffer = device.create_buffer(d.size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY);

		commandBuffer.immediate_submit(sync._uploadFence, [=](VkCommandBuffer cmd) {
			VkBufferCopy copy;
			copy.dstOffset = 0;
			copy.srcOffset = 0;
			copy.size = d.size;
			vkCmdCopyBuffer(cmd, cpu_temp_buffer._buffer, buffer._buffer, 1, &copy);
		});
		vmaDestroyBuffer(device._allocator, cpu_temp_buffer._buffer, cpu_temp_buffer._allocation);
	}
}
